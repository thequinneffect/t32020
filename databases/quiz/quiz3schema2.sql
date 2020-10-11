create table Student (
    zid char(7),
    name text,
    age integer,
    primary key (zid)
);

create table Class (
    code char(4),
    name text,
    primary key (code)
);

create table Enrolment (
    student char(7),
    class char(4),
    mark integer,
    primary key (student, class),
    foreign key (class) references Class(code),
    foreign key (student) references Student(zid)
);