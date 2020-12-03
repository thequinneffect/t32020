-- COMP3311 20T3 Final Exam
-- Q5: find genres that groups worked in

-- ... helper views and/or functions go here ...

drop function if exists q5();
drop type if exists GroupGenres;

create type GroupGenres as ("group" text, genres text);

create or replace function
    q5() returns setof GroupGenres
as $$
declare
    _group groups%rowtype;
    _info GroupGenres;
    _genre text;
begin
    for _group in 
		select * 
		from groups g
	loop 
        _info.group = _group.name;
        _info.genres := '';
        for _genre in 
            select distinct a.genre
            from albums a
            where (a.made_by = _group.id)
            order by a.genre
        loop 
            _info.genres := _info.genres || _genre || ',';
        end loop;
        --_info.genres := substring(info.genres, 1, char_length(info.genres) - 1);
        _info.genres := rtrim(_info.genres, ',');
        return next _info;
    end loop;
end;
$$ language plpgsql
;

