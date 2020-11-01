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

/* Define an SQL view Q2(nstudents,nstaff,nboth) which produces a table 
 with a single row containing counts of:
 - the total number of students (who are not also staff)
 - the total number of staff (who are not also students)
 - the total number of people who are both staff and student 
 
 We only need counts
 - need the students table, staff table
 - I don't believe we need the people table because for the both case
 we can check for each id in student if they are in staff, and for each 
 staff if they are in student.

TODO: try the sum(case when ..) or count(case when ..) approach if it's cleaner
 */