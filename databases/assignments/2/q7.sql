create or replace view convenors_and_courses 
as
select p.name as name, su.code as subject, termname(c.term) as term
    from course_staff s
        join people p on (p.id = s.staff)
        join staff_roles r on (r.id = s.role)
        join courses c on (c.id = s.course)
        join subjects su on (su.id = c.subject)
    where r.name = 'Course Convenor';

create or replace function
   Q7(subject text)
     returns table (subject text, term text, convenor text)
as $$
    select c.subject::text, c.term, c.name
        from convenors_and_courses c
        where c.subject = $1;
$$ language sql;

/* Define an SQL function (not PLpgSQL) called Q7(subject text) that takes 
as parameter a UNSW subject code (e.g. 'COMP1917') and returns a list of 
all offerings of the subject for which a Course Convenor is known.

Note that this means just the Course Convenor role, not Course Lecturer 
or any other role associated with the course. Also, if there happen to be 
several Course Convenors, they should all be returned (in separate tuples). 
If there is no Course Convenor registered for a particular offering of the 
course, then that course offering should not appear in the result.

The function must use the following interface:

create or replace function Q7(subject text) 
  returns table (subject text, term text, convenor text)

The course field in the result tuples should be the UNSW course code 
(i.e. that same thing that was used as the parameter). If the parameter 
does not correspond to a known UNSW course, then simply return an empty 
result set. 

it takes a subject code like COMP1917, which is in the subject table
then we need to look at the offerings of that subject i.e. courses

*/