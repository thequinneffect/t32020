create type RoleEntry as (prog text, term text);

create or replace function
   rolecall(prog text, term text) returns setof RoleEntry
as $$
declare
    _entry RoleEntry;
begin
    for _entry in
    select distinct(p.unswid), p.name
    from people p 
    join program_enrolments e on (e.student = p.id)
    join programs prog on (prog.id = e.program)
    join terms t on (t.id = e.term)
    where prog.code = rolecall.prog and t.name = rolecall.term
    loop 
        return next _entry;
    end loop; 
end;
$$ language plpgsql;

create or replace view Q4a(id,name)
as
select *
from rolecall('3978', 'Sem2 2005');

create or replace view Q4b(id,name)
as
select *
from rolecall('3778', 'Sem1 2017');


/* OLD IMPLEMENTATION
create or replace view Q4a(id,name)
as
select distinct(p.unswid), p.name
from people p 
join program_enrolments e on (e.student = p.id)
join programs prog on (prog.id = e.program)
join terms t on (t.id = e.term)
where prog.code = '3978' and t.name = 'Sem2 2005';

create or replace view Q4b(id,name)
as
select distinct(p.unswid), p.name
from people p 
join program_enrolments e on (e.student = p.id)
join programs prog on (prog.id = e.program)
join terms t on (t.id = e.term)
where prog.code = '3978' and t.name = 'Sem2 2005';
;
*/

/* For the mymy1 database instance ... Define SQL view Q4a(id,name) which 
gives the student IDs and names of all students enrolled in the old 
Computer Science (3978) degree in 05s2.

For the mymy2 database instance ... Define an SQL view Q4b(id,name) which
 gives the student IDs and names of all students enrolled in the Computer 
 Science (3778) degree in 17s1.

Note: the student IDs are the UNSW id's (i.e. student numbers) defined in 
the People.unswid field.

Each of the views will return an empty result in the database for which 
it was not designed.

- we will need the people table due to selecting people.unswid
- we will need the program_enrolments table, it can tell us what program they
are enrolled in 
- we also need the programs table, as it containes the name to filter by */