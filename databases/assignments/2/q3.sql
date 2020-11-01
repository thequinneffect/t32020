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