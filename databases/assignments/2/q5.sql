create or replace view Q5(name)
as
with facs_comms as (
    select f.name as name, count(*) as num
    from orgunits f
    join orgunit_types ft on (f.utype = ft.id)
    join orgunits c on (facultyOf(c.id) = f.id) 
    join orgunit_types ct on (c.utype = ct.id)
    where ft.name = 'Faculty' and ct.name = 'Committee'
    group by f.name
)
select fc.name 
from facs_comms fc
where fc.num >= (
    select max(num) 
    from facs_comms
);