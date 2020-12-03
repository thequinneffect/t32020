-- COMP3311 20T3 Final Exam
-- Q4: list of long and short songs by each group

-- ... helper views and/or functions (if any) go here ...

drop function if exists q4();
drop type if exists SongCounts;
create type SongCounts as ( "group" text, nshort integer, nlong integer );
drop type if exists SongInfo;
create type SongInfo as (title text, length integer);

create or replace function
	q4() returns setof SongCounts
as $$
declare
	_group groups%rowtype;
	_songinfo SongInfo;
	_info SongCounts;
begin
	for _group in 
		select * 
		from groups g
	loop 
		_info.group := _group.name;
		_info.nshort := 0;
		_info.nlong := 0;
		for _songinfo in 
			select distinct s.title, s.length
			from songs s
			join albums a on (a.made_by = _group.id)
			where s.on_album = a.id
		loop
			if (_songinfo.length < 180) then
				_info.nshort := _info.nshort + 1;
			elsif (_songinfo.length > 360) then
				_info.nlong := _info.nlong + 1;
			end if;
		end loop;
		return next _info;
	end loop;
end;
$$ language plpgsql
;
