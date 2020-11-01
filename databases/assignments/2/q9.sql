create or replace function get_recs(aog acad_object_groups)
returns setof AcObjRecord
as $$
declare 
    _expr text;
    _code text;
    _rec acobjrecord;
    dyn_qry text;
begin
    _rec.objtype = aog.gtype; -- should be able to be set just once
    if (aog.gdefby = 'pattern') then 
        -- retrieve all codes from the pattern
        for _expr in
        select * 
        from regexp_split_to_table(aog.definition, ',')
        loop 

            raise info 'regexp: %', _expr;
            -- if it's already a good code, add it
            if (_expr ~ '^[a-zA-Z0-9]*$') then 
                raise info 'found good code: %', _expr;
                _rec.objcode = _expr;
                _rec.objtype = aog.gtype;
                return next _rec;
            else 

                -- either have a {} list or a regex
                if (_expr like '{%}') then 
                    _expr := translate(_expr, '{}', '');
                    raise info '{} list contained %', _expr;
                    for _code in 
                        select * 
                        from regexp_split_to_table(_expr, ';')
                    loop
                        _rec.objcode = _code;
                        _rec.objtype = aog.gtype;
                        return next _rec;
                    end loop;
                else 
                    -- else search relevant aog table for matches
                    _expr := replace(_expr, '#', '.');
                    raise info 'altered regexp %', _expr;

                    dyn_qry := E'
                        select ao.code
                        from ' || quote_ident(aog.gtype || 's') || ' as ao
                        where ao.code ~ ' || quote_literal(_expr);

                    for _code in execute dyn_qry
                    loop
                        raise info 'found % | %', aog.gtype, _code;
                        _rec.objcode = _code;
                        _rec.objtype = aog.gtype;
                        return next _rec;
                    end loop;
                end if;
            end if;
        end loop;
    else -- enumerated
        -- retrieve all codes from the grouping
        raise info 'Enumeration!';

        dyn_qry := '
            select ao.code 
            from ' || quote_ident(aog.gtype || '_group_members') || ' m
            join ' || quote_ident(aog.gtype || 's') || ' ao   
                on (ao.id = m.' || quote_ident(aog.gtype) || ')
            where m.ao_group = ' || quote_literal(aog.id);

        for _code in execute dyn_qry
        loop 
            _rec.objcode = _code;
            return next _rec;
        end loop;

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

/* I'm aware I could have refactored out the group type from 
the get_recs() function because it is the same for all recs. I
pretty much finished coding up the solution though and it's
a lot of effort to change it for not much gain haha.  */