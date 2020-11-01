-- COMP3311 20T3 Assignment 2
-- z5117408

/* NOTE: for style i have chosen to put most new keywords
on a new line, indenting them if they are related to
the above keyword. I'll include comments where I deem 
necessary. */

-- Q1: students who've studied many courses

create view Q1(unswid,name)
as
select p.unswid, p.name
    from people p
    where p.id in ( -- set of students with over 65 courses
        select student
            from course_enrolments
            group by student
                having count(course) > 65);

-- Q2: numbers of students, staff and both

create or replace view Q2(nstudents,nstaff,nboth)
as
with studentstaff as (
    select stu.id as student, sta.id as staff
        from students stu
            full outer join staff sta on (sta.id = stu.id)
            -- full outer gives me all types, student and staff
            -- exclusively staff, or exclusively student just
            -- by checking whether columns are null or not
)
-- now select counts filtering by nulls as explained above
select (
    select count(student)
        from studentstaff
        where staff is null and student is not null
) as nstudents, 
(
    select count(staff)
        from studentstaff
        where student is null and staff is not null
) as nstaff,
(
    select count(*)
        from studentstaff
        where student is not null and staff is not null
) as nboth;

-- Q3: prolific Course Convenor(s)

create or replace view Q3(name,ncourses)
as
with course_convenors as (
    select p.name, count(s) as ncourses -- number of courses convened by this person
        from course_staff s
            join people p on (p.id = s.staff)
            join staff_roles r on (r.id = s.role)
        where r.name = 'Course Convenor'
        group by p.name
)
select *
    from course_convenors c
    where c.ncourses >= ( -- greater or equal to the max convened
        select max(ncourses) 
            from course_convenors);

-- Q4: Comp Sci students in 05s2 and 17s1

/* I decided to implment a function because I noticed they queries
for mymy1 and mymy2 would be the same (except for the parameters) and
views cannot be parameterised. Functions like below are analogous to
parameterised views. Just in case we weren't supposed to use a function
here, i've included commented out view only versions. */

create type RoleEntry as (prog text, term text);

create or replace function
    rolecall(prog text, term text) returns setof RoleEntry
as $$
declare
    _entry RoleEntry;
begin
    for _entry in
        select distinct(p.unswid), p.name
            from people p 
                join program_enrolments e on (e.student = p.id)
                join programs prog on (prog.id = e.program)
                join terms t on (t.id = e.term)
            where prog.code = rolecall.prog and t.name = rolecall.term
    loop 
        return next _entry;
    end loop; 
end;
$$ language plpgsql;

create or replace view Q4a(id,name)
as
select *
    from rolecall('3978', 'Sem2 2005');

create or replace view Q4b(id,name)
as
select *
    from rolecall('3778', '2017 S1');

/* VIEW-ONLY IMPLEMENTATION
create or replace view Q4a(id,name)
as
select distinct(p.unswid), p.name
    from people p 
        join program_enrolments e on (e.student = p.id)
        join programs prog on (prog.id = e.program)
        join terms t on (t.id = e.term)
    where prog.code = '3978' and t.name = 'Sem2 2005';

create or replace view Q4b(id,name)
as
select distinct(p.unswid), p.name
    from people p 
        join program_enrolments e on (e.student = p.id)
        join programs prog on (prog.id = e.program)
        join terms t on (t.id = e.term)
    where prog.code = '3778' and t.name = '2017 S1';
*/

-- Q5: most "committee"d faculty

create or replace view Q5(name)
as
with facs_comms as (
    select f.name as name, count(*) as num -- only interested in the count
        from orgunits f
            join orgunit_types ft on (f.utype = ft.id)
            join orgunits c on (facultyOf(c.id) = f.id) -- must be a child of faculty
            join orgunit_types ct on (c.utype = ct.id)
        where ft.name = 'Faculty' and ct.name = 'Committee' -- must be faculties and committees only
        group by f.name
)
select fc.name 
    from facs_comms fc
    where fc.num >= ( -- number of committees is >= all other faculties
        select max(num) 
        from facs_comms);

-- Q6: nameOf function

create or replace function
    Q6(id integer) returns text
as $$
    select p.name 
        from people p 
        where p.unswid = $1 or p.id = $1; 
        -- can only match one, by the spec
$$ language sql;

-- Q7: offerings of a subject

-- helper view that pairs courses and course convenors
create or replace view convenors_and_courses 
as
select p.name as name, su.code as subject, termname(c.term) as term
    from course_staff s
        join people p on (p.id = s.staff)
        join staff_roles r on (r.id = s.role)
        join courses c on (c.id = s.course) -- for term
        join subjects su on (su.id = c.subject) -- for code
    where r.name = 'Course Convenor';

create or replace function
   Q7(subject text)
     returns table (subject text, term text, convenor text)
as $$
    select c.subject::text, c.term, c.name
        from convenors_and_courses c
        where c.subject = $1;
$$ language sql;

-- Q8: transcript

create or replace function
    Q8(zid integer) returns setof TranscriptRecord
as $$
declare 
    pid people.id%type;
    rec TranscriptRecord;
    summary TranscriptRecord; -- final line of transcript
    g course_enrolments.grade%type;
    weightedSumOfMarks float := 0.0;
    uocPassed integer := 0;
    uocAttempted integer := 0;
    markMatters boolean := false;
    uocMatters boolean := false;
begin 

    -- check if the student exists
    select p.id
    from people p
    where p.unswid = zid
    into pid;

    if (pid is null) then
        raise exception 'Invalid student %', zid;
    end if;

    summary.name := 'No WAM available'; -- default value

    -- get courses completed
    found := false;
    for rec in
        select s.code, termname(c.term) as term, prog.code as prog, substring(s.name, 1, 20) as name, ce.mark, ce.grade, s.uoc 
            from people p
                join course_enrolments ce on (ce.student = p.id)
                join courses c on (c.id = ce.course)
                join subjects s on (s.id = c.subject)
                join program_enrolments pe on (pe.term = c.term and pe.student = p.id)
                join programs prog on (prog.id = pe.program)
                join terms t on (t.id = c.term)
            where p.unswid = zid
            order by t.starting, s.code
    loop 

        -- check if the students mark matters in terms of wam
        select true where rec.grade in (
            'PT', 'PC', 'PS', 'CR', 'DN', 
            'HD', 'A', 'B', 'C', 'RC', 'RS'
        ) into markMatters;
        -- check if the grade matters in terms of uoc
        select true where rec.grade in (
            'SY', 'XE', 'T', 'PE'
        ) into uocMatters;
        
        if (markMatters) then -- see if mark matters, if it does uoc also does
            uocAttempted := uocAttempted + rec.uoc;
            uocPassed := uocPassed + rec.uoc;
            weightedSumOfMarks := weightedSumOfMarks + (rec.mark::float * rec.uoc);
        elsif (uocMatters) then -- see if at least uoc matters
            uocPassed := uocPassed + rec.uoc;
        else -- failed
            uocAttempted := uocAttempted + rec.uoc; -- failed uoc is still attempted
            -- mark also still counts for wam
            weightedSumOfMarks := weightedSumOfMarks + (rec.mark::float * rec.uoc);
            rec.uoc := null;
        end if;

        uocMatters := false; -- reset mark and uoc for next loop
        markMatters := false;
        return next rec;
        
    end loop;

    -- calculate final WAM if there was some uocAttempted
    if (uocAttempted != 0) then
        summary.name := 'Overall WAM/UOC';
        summary.mark := round(weightedSumOfMarks / uocAttempted)::integer;
        summary.uoc := uocPassed;
    end if;
    
    return next Summary;

end;
$$ language plpgsql;

-- Q9: members of academic object group

create or replace function 
    get_recs(aog acad_object_groups) returns setof AcObjRecord
as $$
declare 
    _expr text; -- stores pattern definition for pattern types
    _code text; -- stores academic object codes (course, subject, stream)
    _rec acobjrecord;
    _dyn_qry text; -- dynamic query holder
begin
    -- this set will always have the same academic object type, so set once here
    _rec.objtype = aog.gtype;

    -- handle the two types separately, pattern vs enumerated
    if (aog.gdefby = 'pattern') then 
        
        -- for each regex in the pattern
        for _expr in
            select * 
                from regexp_split_to_table(aog.definition, ',')
        loop 
            -- if it's already a good code, add it
            if (_expr ~ '^[a-zA-Z0-9]*$') then 
                _rec.objcode = _expr;
                return next _rec;
            else 
                -- either have a {} list or a regex
                if (_expr like '{%}') then 
                    -- have {}, so remove them
                    _expr := translate(_expr, '{}', '');
                    -- return codes in set
                    for _code in 
                        select * 
                            from regexp_split_to_table(_expr, ';')
                    loop
                        _rec.objcode = _code;
                        return next _rec;
                    end loop;

                else -- REGEX case
                    -- convert to POSIX regex style
                    _expr := replace(_expr, '#', '.');

                    -- generate query specific to academic object types table
                    _dyn_qry := E'
                        select ao.code
                        from ' || quote_ident(aog.gtype || 's') || ' as ao
                        where ao.code ~ ' || quote_literal(_expr);

                    -- add codes that exist in that table
                    for _code in execute _dyn_qry
                    loop
                        _rec.objcode = _code;
                        return next _rec;
                    end loop;
                end if;
            end if;
        end loop;
    else -- enumerated case

        -- generate query specific to academic object type table
        _dyn_qry := '
            select ao.code 
            from ' || quote_ident(aog.gtype || '_group_members') || ' m
            join ' || quote_ident(aog.gtype || 's') || ' ao   
                on (ao.id = m.' || quote_ident(aog.gtype) || ')
            where m.ao_group = ' || quote_literal(aog.id);

        -- return codes found
        for _code in execute _dyn_qry
        loop 
            _rec.objcode = _code;
            return next _rec;
        end loop;

    end if; 
end;
$$ language plpgsql;

create or replace function
    get_groups(gid integer) returns setof AcObjRecord
as $$
declare
    _parent acad_object_groups;
    _child acad_object_groups;
    _rec acobjrecord;
begin 
    -- try retrieve parent of group
    select *
        from acad_object_groups g
        where g.id = get_groups.gid
            and g.negated != true 
            and g.gdefby != 'query'
        into _parent;

    -- check if parent was found, except if not
    if (not found) then 
        raise exception 'No such group %', get_groups.gid;
    end if;

    -- parent found, include its info in result set
    for _rec in 
        select *
            from get_recs(_parent)
    loop 
        return next _rec;
    end loop;

    -- loop through children of parent, also including recs in result set
    for _child in 
        select *
        from acad_object_groups g 
        where g.parent = _parent.id
            and g.negated != true
            and g.gdefby != 'query'
    loop
        for _rec in 
            select * 
                from get_recs(_child)
        loop 
            return next _rec;
        end loop;
    end loop;
    
end;
$$ language plpgsql;

create or replace function
   Q9(gid integer) returns setof AcObjRecord
as $$
begin
    -- get only the distinct entries
    return query 
        select distinct *
            from get_groups(gid) recs;
end;
$$ language plpgsql;

/* After coding this up, i saw a post on the forum stating that
children academic object groups should always be the same type 
as the parent. This would mean I could just set _rec.objtype
once in the get_groups function and only return codes from
get_recs, but this is a lot to change after the fact for no
real gain. You could even argue that this is better because it's 
open for the possibility of grouping different kinds of academic
objects if that is ever needed. */

-- Q10: follow-on courses

create or replace function
    Q10(code text) returns setof text
as $$
declare
    _code text;
begin 
    -- search for the supplied course code in the pattern 
    -- for other courses lists of prereqs
    for _code in
        select distinct s.code
        from acad_object_groups aog
            join rules r on (r.ao_group = aog.id)
            join subject_prereqs sp on (sp.rule = r.id)
            join subjects s on (s.id = sp.subject)
        where aog.gdefby = 'pattern' 
            and aog.definition like ('%' || Q10.code || '%')
    loop
        return next _code;
    end loop;
end;
$$ language plpgsql;
