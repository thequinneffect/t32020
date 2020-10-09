-- COMP3311 Prac 03 Exercise
-- Schema for simple company database

create domain depid as char(3) check (val ~ '[0-9]{3}');

create table Employees (
	tfn         char(11) check (tfn ~ '[0-9]{3}-[0-9]{3}-[0-9]{3}'),
	givenName   varchar(30) not null,
	familyName  varchar(30),
	hoursPweek  float check (hoursPweek >= 0 and
								hoursPweek <= 168),
	primary key (tfn)
);

create table Departments (
	id          depid,
	name        varchar(100) unique,
	manager     char(11) unique not null,
	primary key (id),
	foreign key (manager) references (Employees.tfn)
);

create table DeptMissions (
	department  depid,
	keyword     varchar(20),
	primary key (department, keyword),
	foreign key (department) references (Departments.id)
);

create table WorksFor (
	employee    char(11),
	department  depid,
	percentage  float check (percentage > 0.0 and
								percentage <= 1.0),
	primary key (employee, department),
	foreign key (employee) references (Employees.tfn),
	foreign key (department) references (Departments.id)
);
