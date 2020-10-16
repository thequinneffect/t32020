create table enrolments (
    student text,
    course  text,
    mark    integer check (mark between 0 and 100),
    grade   char(1) check (grade between 'A' and 'E'),
    primary key (student,course)
);

insert into enrolments values ('james', 'COMP1917 12s1', 50, 'D');
insert into enrolments values ('james', 'COMP1927 12s2', 55, 'D');
insert into enrolments values ('james', 'COMP2911 13s1', 50, 'D');
insert into enrolments values ('peter', 'COMP1917 12s1', 45, 'E');
insert into enrolments values ('peter', 'COMP1917 12s2', 40, 'E');
insert into enrolments values ('john', 'COMP1917 12s1', 90, 'A');
insert into enrolments values ('john', 'COMP1927 12s2', 85, 'A');
insert into enrolments values ('john', 'COMP2911 13s1', 85, 'A');
insert into enrolments values ('john', 'COMP3311 13s2', 70, 'B');
