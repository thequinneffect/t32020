create or replace function 
    get_recs(aog acad_object_groups) returns setof AcObjRecord
as $$
declare 
    _expr text; -- stores pattern definition for pattern types
    _code text; -- stores academic object codes (course, subject, stream)
    _rec acobjrecord;
    _dyn_qry text; -- dynamic query holder
begin
    -- this set will always have the same academic object type, so set once here
    _rec.objtype = aog.gtype;

    -- handle the two types separately, pattern vs enumerated
    if (aog.gdefby = 'pattern') then 
        
        -- for each regex in the pattern
        for _expr in
            select * 
                from regexp_split_to_table(aog.definition, ',')
        loop 
            if (_expr like '%FREE%' or _expr like '%GEN%' or _expr like '%F=%') then
                continue;
            -- if it's already a good code, add it
            elsif (_expr ~ '^[a-zA-Z0-9]*$') then 
                _rec.objcode = _expr;
                return next _rec;
            else 
                -- either have a {} list or a regex
                if (_expr like '{%}') then 
                    -- have {}, so remove them
                    _expr := translate(_expr, '{}', '');
                    -- return codes in set
                    for _code in 
                        select * 
                            from regexp_split_to_table(_expr, ';')
                    loop
                        _rec.objcode = _code;
                        return next _rec;
                    end loop;

                else -- REGEX case
                    -- convert to POSIX regex style
                    _expr := replace(_expr, '#', '.');

                    -- generate query specific to academic object types table
                    _dyn_qry := E'
                        select ao.code
                        from ' || quote_ident(aog.gtype || 's') || ' as ao
                        where ao.code ~ ' || quote_literal(_expr);

                    -- add codes that exist in that table
                    for _code in execute _dyn_qry
                    loop
                        _rec.objcode = _code;
                        return next _rec;
                    end loop;
                end if;
            end if;
        end loop;
    else -- enumerated case

        -- generate query specific to academic object type table
        _dyn_qry := '
            select ao.code 
            from ' || quote_ident(aog.gtype || '_group_members') || ' m
            join ' || quote_ident(aog.gtype || 's') || ' ao   
                on (ao.id = m.' || quote_ident(aog.gtype) || ')
            where m.ao_group = ' || quote_literal(aog.id);

        -- return codes found
        for _code in execute _dyn_qry
        loop 
            _rec.objcode = _code;
            return next _rec;
        end loop;

    end if; 
end;
$$ language plpgsql;

create or replace function
    get_groups(gid integer) returns setof AcObjRecord
as $$
declare
    _parent acad_object_groups;
    _child acad_object_groups;
    _rec acobjrecord;
begin 
    -- try retrieve parent of group
    select *
        from acad_object_groups g
        where g.id = get_groups.gid
            and g.negated != true 
            and g.gdefby != 'query'
        into _parent;

    -- check if parent was found, except if not
    if (not found) then 
        raise exception 'No such group %', get_groups.gid;
    end if;

    -- parent found, include its info in result set
    for _rec in 
        select *
            from get_recs(_parent)
    loop 
        return next _rec;
    end loop;

    -- loop through children of parent, also including recs in result set
    for _child in 
        select *
        from acad_object_groups g 
        where g.parent = _parent.id
            and g.negated != true
            and g.gdefby != 'query'
    loop
        for _rec in 
            select * 
                from get_recs(_child)
        loop 
            return next _rec;
        end loop;
    end loop;
    
end;
$$ language plpgsql;

create or replace function
   Q9(gid integer) returns setof AcObjRecord
as $$
begin
    -- get only the distinct entries
    return query 
        select distinct *
            from get_groups(gid) recs;
end;
$$ language plpgsql;

/* After coding this up, i saw a post on the forum stating that
children academic object groups should always be the same type 
as the parent. This would mean I could just set _rec.objtype
once in the get_groups function and only return codes from
get_recs, but this is a lot to change after the fact for no
real gain. You could even argue that this is better because it's 
open for the possibility of grouping different kinds of academic
objects if that is ever needed. */