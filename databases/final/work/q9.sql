/*
create or replace function
    checkDisbanded() returns trigger
as $$
declare
    _members_left integer;
begin
    _members_left := 0;
    select count(*)
    into _members_left
    from memberof m
    where m.in_group = old.in_group;
    if (_members_left = 0) then
        update groups g
        set disbanded = old.departed
        where g.id = old.in_group;
    end if;
    return old;
end;
$$ language plpgsql
;
*/
create trigger onNameChange
before update on groups.name 
execute procedure onNameChange();

create or replace function
    onNameChange() returns trigger
as $$
declare
    _newid integer;
    _mid integer;
begin
    -- insert new group
    _newid = newGroupId();
    insert into groups (id, name, formed)
    values (_newid, new.name, current_date);

    -- add members to new group
    for _mid in 
        select performer
        from memberof m
        where m.in_group = old.id
    loop
        insert into memberof (in_group, performer)
        values (_newid, _mid);
    end loop;

    -- remove members from old group
    delete from memberof m
    where m.in_group = old.id;

    -- set the old group as disbanded
    old.disbanded = current_date;
    -- results in no change in name, but change in disbanded for the old tuple
    return old; 
end;
$$ language plpgsql
;
