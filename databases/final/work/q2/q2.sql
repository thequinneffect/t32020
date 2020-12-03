-- COMP3311 20T3 Final Exam
-- Q2: group(s) with no albums

-- ... helper views (if any) go here ...

create or replace view q2("group")
as
select g.name
from groups g
left join albums a on (a.made_by = g.id)
where a.title is null
;

