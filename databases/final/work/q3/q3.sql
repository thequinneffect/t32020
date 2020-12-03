-- COMP3311 20T3 Final Exam
-- Q3:  performer(s) who play many instruments

-- ... helper views (if any) go here ...

create or replace view num_guitars(nguitars)
as
select count(distinct p.instrument)
from playson p
where p.instrument ilike '%guitar%'
;

create or replace view num_instruments(ninstruments)
as
select count(distinct p.instrument) - (select * from num_guitars) + 1
from playson p
where p.instrument <> 'vocals';
;

create or replace view all_instruments(instrument)
as
select distinct p.instrument 
from playson p
where p.instrument <> 'vocals';
;

create or replace view q3(performer,ninstruments)
as
...put your SQL here...
;

