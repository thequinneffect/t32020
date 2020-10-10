-- COMP3311 20T3 Assignment 1
-- Calendar schema
-- Written by INSERT YOUR NAME HERE

-- Types

create type AccessibilityType as enum ('read-write','read-only','none');
create type InviteStatus as enum ('invited','accepted','declined');

-- add more types/domains if you want

-- Entity tables

/*
Users
- individuals who use the calendar
- we need to know at least their name and email address
- they also have a non-empty password for authentication
- some (very few) users have administration privileges
(we don't need to specify what these privileges are in the data model)
*/
create table Users (
	id          serial,
	name        text not null,
	email       text not null unique,
	passwd      text not null,
	is_admin    boolean not null default false, -- TODO: default to false? not null?
	primary key (id)
);

/*
Groups
- named collections of individual users
- useful as shorthand for scheduling events for specific groups
- each group is owned by a user, who maintains the list of members
*/
create table Groups (
	id          serial,
	name        text not null,
	owner       integer not null, -- total participation
	primary key (id),
	foreign key (owner) references Users(id)
);

/*
Calendars
- named collections of events (e.g. "John's Weekly Meetings/Classes")
- each event is attached to a specific calendar
- each calendar has accessibility restrictions (per user and default)
(e.g. some users have read/write, some have read-only, some have no access)
- if a user has read permission on a calendar, they see private event titles 
instead of "Busy"
- each calendar is owned by a user; a user may own many calendars;
- users may subscribe to other peoples' calendars (if they can read them)
- each calendar has a colour (set by its owner); a subscriber may set a 
different colour for their own view
*/
create table Calendars (
	id             serial,
	name           text not null, -- unique?
	default_access AccessibilityType,
	-- TODO: colour
	owner          integer, -- total participation, 1-many
	primary key    (id),
	foreign key    (owner) references Users(id)
);

create table Events (
	id          serial,
	part_of     integer not null, -- total participation
	created_by  integer not null, -- total participation
	primary key (id),
	foreign key (part_of) references Calendars(id),
	foreign key (created_by) references Users(id)
);


-- Relationship tables

create table Member (
	user_id     integer,
	group_id    integer,
	primary key (user_id, group_id),
	foreign key (user_id) references Users(id),
	foreign key (group_id) references Groups(id)
);

/* The AccessibilityType enum covers all sensible values for accessibility. There
 * is no case where it is useful to have null here, and hence I have made it 
 * not null. The whole tuple would be useless if it were null as that's the only
 * useful piece of information stored on this relationship. */
create table Accessibility (
	user_id     integer,
	calendar_id integer,
	access      AccessibilityType not null, -- whole tuple is useless if this is null
	primary key (user_id, calendar_id),
	foreign key (user_id) references Users(id),
	foreign key (calendar_id) references Calendars(id)
);

create table Subscribed (
	user_id     integer,
	calendar_id integer,
	-- TODO: colour
	primary key (user_id, calendar_id),
	foreign key (user_id) references Users(id),
	foreign key (calendar_id) references Calendars(id)
);

/* When a user is initially invited to an event, the event should start with the value
 * invited (not null). The user will either accept or decline, in which case the value
 * will become accepted or declined respectively, and hence it is never null. Therefore
 * i add the not null constraint. Furthermore, a tuple with status set to null would
 * offer no information and be useless. */
create table Invited (
	user_id     integer,
	event_id    integer,
	status      InviteStatus not null, -- whole tuple is useless if this is null
	primary key (user_id, event_id),
	foreign key (user_id) references Users(id),
	foreign key (event_id) references Events(id)
);

-- etc. etc. etc.
