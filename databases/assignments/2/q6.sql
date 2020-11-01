create or replace function
    Q6(id integer) returns text
as $$
    select p.name 
        from people p 
        where p.unswid = $1 or p.id = $1; -- can only match one, by the spec
$$ language sql;