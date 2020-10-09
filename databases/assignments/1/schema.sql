-- COMP3311 20T3 Assignment 1
-- Calendar schema
-- Written by INSERT YOUR NAME HERE

-- Types

create type AccessibilityType as enum ('read-write','read-only','none');
create type InviteStatus as enum ('invited','accepted','declined');

-- add more types/domains if you want

-- Tables

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
	is_admin    boolean, -- default to false? not null?
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
	...
	primary key (id)
);

create table Calendars (
	id          serial,
	primary key (id)
);

create table Events (
	id          serial,
	primary key (id)
);

-- etc. etc. etc.
