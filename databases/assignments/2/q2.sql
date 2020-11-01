create or replace view Q2(nstudents,nstaff,nboth)
as
with studentstaff as (
    select stu.id as student, sta.id as staff
        from students stu
            full outer join staff sta on (sta.id = stu.id)
)
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