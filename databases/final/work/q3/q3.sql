-- COMP3311 20T3 Final Exam
-- Q3:  performer(s) who play many instruments

-- ... helper views (if any) go here ...

create or replace view num_instruments(ninstruments)
as
select count(distinct regexp_replace(p.instrument, '.*guitar.*', 'guitar'))
from playson p
where p.instrument <> 'vocals'
;

create or replace view q3(performer,ninstruments)
as
select distinct p.name, count(distinct regexp_replace(po.instrument, '.*guitar.*', 'guitar'))
from performers p
join playson po on (po.performer = p.id)
where po.instrument <> 'vocals'
group by p.id, p.name
having count(distinct regexp_replace(po.instrument, '.*guitar.*', 'guitar')) > (select ninstruments/2 from num_instruments)
;
