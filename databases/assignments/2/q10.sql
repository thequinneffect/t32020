create or replace function
   Q10(code text) returns setof text
as $$
declare
    _code text;
begin 
    for _code in
    select distinct s.code
    from acad_object_groups aog
        join rules r on (r.ao_group = aog.id)
        join subject_prereqs sp on (sp.rule = r.id)
        join subjects s on (s.id = sp.subject)
    where aog.gdefby = 'pattern' 
        and aog.definition like ('%' || Q10.code || '%')
    loop
        return next _code;
    end loop;
end;
$$ language plpgsql;