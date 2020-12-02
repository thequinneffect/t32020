-- COMP3311 20T3 Final Exam
-- Q1: longest album(s)

-- ... helper views (if any) go here ...

create or replace view longest_album_length(s_length)
as
select sum(s.length)
from songs s
group by s.on_album
order by sum(s.length) desc
limit 1
;

create or replace view q1("group",album,year)
as
select g.name, a.title, a.year
from albums a
join groups g on (g.id = a.made_by)
join songs s on (s.on_album = a.id)
group by a.id, g.name, a.title, a.year
having sum(s.length) >= (select * from longest_album_length);
;

