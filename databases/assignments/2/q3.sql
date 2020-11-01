create or replace view Q3(name,ncourses)
as
with course_convenors as (
    select p.name, count(s) as ncourses
    from course_staff s
    join people p on (p.id = s.staff)
    join staff_roles r on (r.id = s.role)
    where r.name = 'Course Convenor'
    group by p.name
)
select *
from course_convenors c
where c.ncourses >= (select max(ncourses) from course_convenors);

/* Define an SQL view Q3(name,ncourses) that prints the name of the 
 person(s) who has been lecturer-in-charge (LIC) of the most courses at
UNSW and the number of courses they have been LIC for. In the database, 
the LIC has the role of "Course Convenor".

 we need all staff, then we need to group them up by their identity,
 then we need to filter just to where their role is LIC,
 then we need to output the >= all others of that same set for the 
 amount of courses they were LIC for

 The affiliation gives us the staff id and the role id. This allows
 us to connect roles (LIC) to staff (names). The count of affilitations
 gives us how many courses they were LIC for.
 */