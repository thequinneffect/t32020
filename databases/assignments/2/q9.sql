create or replace function get_recs(aog acad_object_groups)
returns setof AcObjRecord
as $$
declare 
    _expr text;
    _code text;
    _rec acobjrecord;
    _get_og_codes text;
begin
    if (aog.gdefby = 'pattern') then 
        -- retrieve all codes from the pattern
        for _expr in
        select * 
        from regexp_split_to_table(aog.definition, ',')
        loop 

            raise info 'regexp: %', _expr;
            _expr := regexp_replace(_expr, '#', '.');
            raise info 'altered regexp %', _expr;

            _get_og_codes := E'
                select ao.code
                from ' || quote_ident(aog.gtype || 's') || ' as ao
                where ao.code ~ ' || quote_literal(_expr);

            for _code in execute _get_og_codes
            loop
                _rec.objcode = _code;
                _rec.objtype = aog.gtype;
            end loop;
        end loop;
    else -- enumerated
        -- retrieve all codes from the grouping
        raise exception 'TODO: implement enumerated type logic!';
    end if; 
end;
$$ language plpgsql;

/*
    regardless of whether it is a program, stream, or subjects grouping,
    we will largely do the same thing for groups of the same type
    i.e. regex vs enumerated
    branch based on regex or enumerated, then use dynamic sql queries
    to do roughly the same logic for each type (program, stream, subject)
 */

create or replace function
   Q9(gid integer) returns setof AcObjRecord
as $$
declare
    _name text;
    _gtype text;
    _parent acad_object_groups;
    _child acad_object_groups;
    _rec acobjrecord;
begin 
    -- try retrieve parent of group
    select *
    from acad_object_groups g
    where g.id = Q9.gid
        and g.negated != true 
        and g.gdefby != 'query'
    into _parent;

    -- check if parent was found, except if not
    if (not found) then 
        raise exception 'No such group %', Q9.gid;
    end if;

    -- parent found, include it in result set
    for _rec in 
    select *
    from get_recs(_parent)
    loop 
        return next _rec;
    end loop;

    -- loop through children of parent
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