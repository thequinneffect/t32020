create or replace view Q5(name)
as
with facs_comms as (
    select f.name as name, count(*) as num -- only interested in the count
        from orgunits f
            join orgunit_types ft on (f.utype = ft.id)
            join orgunits c on (facultyOf(c.id) = f.id) -- must be a child of faculty
            join orgunit_types ct on (c.utype = ct.id)
        where ft.name = 'Faculty' and ct.name = 'Committee' -- must be faculties and committees only
        group by f.name
)
select fc.name 
    from facs_comms fc
    where fc.num >= ( -- number of committees is >= all other faculties
        select max(num) 
        from facs_comms);