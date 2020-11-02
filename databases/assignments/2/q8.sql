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
            if (rec.uoc is not null) then 
                uocPassed := uocPassed + rec.uoc;
            end if;
            -- it shouldn't be null in this case, but just in case
            if (rec.mark is not null and rec.uoc is not null) then 
                weightedSumOfMarks := weightedSumOfMarks + (rec.mark::float * rec.uoc);
                uocAttempted := uocAttempted + rec.uoc;
            end if;
        elsif (uocMatters) then
            if (rec.uoc is not null) then 
                uocPassed := uocPassed + rec.uoc;
            end if;
        else -- failed
            if (rec.uoc is not null and rec.mark is not null) then
                uocAttempted := uocAttempted + rec.uoc; -- failed uoc is still attempted
            end if;
            -- but mark still counts for wam
            if (rec.mark is not null and rec.uoc is not null) then
                weightedSumOfMarks := weightedSumOfMarks + (rec.mark::float * rec.uoc);
            end if;
            -- don't display UOC if they failed
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