create or replace function
   Q6(id integer) returns text
as $$
    select p.name 
    from people p 
    where p.unswid = $1 or p.id = $1;
$$ language sql;

/* Define an SQL function (SQL, not PLpgSQL) called Q6(id integer) that 
takes as parameter either a People.id value (i.e. an internal database 
identifier) or a People.unswid value (i.e. a UNSW student ID), and returns 
the name of that person. If the id value is invalid, return NULL as the 
result. You can assume that People.id and People.unswid values come from 
disjoint sets, so you should never return multiple names. (It turns out 
that the sets aren't quite disjoint, but I won't test any of the cases 
where the id/unswid sets overlap)

The function must use the following interface:
create or replace function Q6(id integer) returns text ... */