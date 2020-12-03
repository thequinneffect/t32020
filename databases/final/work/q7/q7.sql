-- COMP3311 20T3 Final Exam Q7
-- Put any helper views or PLpgSQL functions here
-- You can leave it empty, but you must submit it

drop function if exists instrumentsOf(_performer integer, _song integer);

create or replace function
    instrumentsOf(_performer integer, _song integer) returns text
as $$
declare
    _instrument text;
    _all text := '';
begin
    for _instrument in 
        select distinct p.instrument 
        from playson p
        where p.song = _song and p.performer = _performer
        order by p.instrument
    loop
        _all := _all || _instrument || ',';
    end loop;
    _all := rtrim(_all, ',');
    return _all;
end;
$$ language plpgsql
;
