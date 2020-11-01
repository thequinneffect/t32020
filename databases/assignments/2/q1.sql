/* Note: for style i have chosen to put most new keywords
on a new line, indenting them if they are related to
the above keyword. I'll include comments where I deem 
necessary. */

create view Q1(unswid,name)
as
select p.unswid, p.name
    from people p
    where p.id in ( -- set of students with over 65 courses
        select student
            from course_enrolments
            group by student
                having count(course) > 65);