create view Q1(unswid,name)
as
select p.unswid, p.name
from people p
where p.id in (
    select student
    from course_enrolments
    group by student
    having count(course) > 65
);

/* Define an SQL view Q1(unswid,name) that gives the student id and name
 of any student who has studied more than 65 courses at UNSW. The name 
 should be take from the People.name field for the student, and the 
 student id should be taken from People.unswid.
 
 Tables needed;
 - people (for extracting unswid and name)
 - course_enrolments (for determining which courses people have done)
 - need to group by the name or unswid in the course_enrolment, then 
 count(course) to see how many courses they have done, then filter
 this to be > 65

 select p.unswid, p.name
 from people p
 where p.id in (select student
 from course_enrolments
 group by student
 having count(course) > 65);
 */