-- COMP3311 20T3 Assignment 1
-- Calendar schema
-- Written by Nicholas Quinn (z5117408)

-- Types

create type Accessibility_type as enum ('read-write','read-only','none');
create type Invite_status as enum ('invited','accepted','declined');
create type Visibility_type as enum ('public', 'private');
create type Day_of_week as enum ('mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun');

-- Entity tables

/* The reason I chose is_admin to be not null is because whilst it says
 * "some (very few) user have ...", that is in relation to admin privileges
 * specifically. This doesn't actually say whether the field itself may
 * or may not be null, just that when it isn't null, it's most likely
 * false (hence the default to false as well). I've not put any character
 * constraints on the name because it's ambigious if the application
 * will allow users to use numbers, special chars, etc. in their names. */
create table Users (
	id          serial,
	name        text    not null,                                 -- need to know, not may know
	email       text    not null unique check (email like '%@%'), -- need to know
	passwd      text    not null,                                 -- they have, not may have
	is_admin    boolean not null default false,                   -- see block comment
	primary key (id)
);

create table Groups (
	id          serial,
	name        text    not null, -- not unique, 2 users can have a group each of the same name
	owner       integer not null, -- total participation
	unique      (name, owner),    -- user cannot have 2 groups of the same name
	primary key (id),
	foreign key (owner) references Users(id)
);

/* I haven't put character range constraints on colour because some
 * colour formats use 0x.., other could just write the colour. So it's
 * clear some need digits and others don't, best to leave it as is. */
create table Calendars (
	id             serial,
	name           text               not null,                -- not unique, 2 users can have same named calendar
	default_access Accessibility_type not null default 'none', -- has, not may have
	colour         text               not null,
	owner          integer            not null,                -- total participation, 1-many
	unique         (name, owner)                               -- user cannot own 2 calendars of the same name
	primary key    (id),
	foreign key    (owner) references Users(id)
);

/* (part_of, title) and (created_by, title) are not unique because it makes
 * sense that a user would want to have multiplle events of the same name,
 * perhaps putting them on the same calendar. I've also made it so that
 * you can either have or not have a start time, but if you don't have a
 * start time you can't have an end time. A null end time just means the
 * program will let this event run until the end of that day. */
create table Events (
	id          serial,
	title       text            not null,                   -- has, not may have
	visibility  Visibility_type not null default 'private', -- has
	location    point,                                      -- may be associated with a location, not is
	start_time  time,
	end_time    time,
	part_of     integer         not null,                   -- total participation
	created_by  integer         not null,                   -- total participation
	check       (start_time < end_time),
	check       (start_time is not null or (start_time is null and end_time is null)),
	primary key (id),
	foreign key (part_of) references Calendars(id),
	foreign key (created_by) references Users(id)
);

/* I've chosen to use interval for alarms because if you store time
 * then you have to check if the alarm time is before the event time.
 * The time that the alarm notification will be triggered can be derived
 * from the event time and the interval, in the program using this db. */
create table Alarms (
	event_id    integer,
	alarm       interval, -- not null already implicity applied from pkey
	primary key (event_id, alarm),
	foreign key (event_id) references Events(id)
);

/* For the following subclasses of the Events table, there is no way to
 * capture the disjoint aspect of the ER diagram. This is because we
 * could make one of each of them and have them all reference back
 * to the same Events tuple/instance (which would break the disjointedness).
 * There is no constraint to fix this. Triggers would work, but they are 
 * out of the scope of this assignment.
 * The total participation (every event must be one of the sub-types), is
 * captured automatically by the nature of how foreign keys work i.e. to 
 * create an Events sub-type, the foreign key must be present in an 
 * Events tuple already, and hence every event sub-type will also be an 
 * Event, otherwise it wouldn't have been allowed to be inserted. */

create table One_day_events (
	event_id    integer,
	date 		date    not null, -- a day event without a date is useless
	primary key (event_id),
	foreign key (event_id) references Events(id)
);

/* Unlike the Recurring_events.end_date, this end_date was not specified to be
 * able to be null in the spec. Also, even though end_date can be null in
 * recurring events, it will still terminate due to n_times being completed
 * in the case that it is null. Furthermore, from a GUI point of view, an event
 * that spans infinitely could be problematic. I think the intended use case here
 * is to have events that span days, weeks, months, years, maaaybe decades, but
 * not really infinitely. */
create table Spanning_events (
	event_id    integer,
	start_date  date    not null,
	end_date    date    not null check (end_date > start_date), -- if = you would use a one day event
	primary key (event_id),
	foreign key (event_id) references Events(id)
);

/* I've made n_times nullable because a recurring event tuple
 * without n_times set could mean it recurs indefinetely until deleted.
 * I made the check >= 1 and not 2 because it could be n_times to recur, not
 * how many times it will occur in total e.g. n_times = 1 = recur once = 2 times total.*/
create table Recurring_events (
	event_id    integer,
	start_date  date    not null,                              -- will have a starting date
	end_date    date    check (end_date > start_date),         -- may have an ending date
	n_times     integer check (n_times >= 1),                  -- not a recurring event otherwise
	check       (end_date is not null or n_times is not null), -- only need one to terminate really
	primary key (event_id),
	foreign key (event_id) references Events(id)
);

/* See the comment above the OneDayEvents table as the argument for
 * why we cannot capture the disjoint property of the ER diagram,
 * and how the total participation works, for the different sub-types 
 * of Recurring_events is pretty much the same. */

/* I have made frequency/day_of_week not null and >= 1 because there is 
 * no point allowing a weekly event to be created if it doesn't 
 * provide the necessary information to run it. Both the day of week
 * and frequency are needed to be known for the tuple to make sense.
 * Otherwise you would just store it as a normal OneDayEvent. */
create table Weekly_events (
	recurring_event integer,
	day_of_week     Day_of_week not null, -- need to know which day event is on
	frequency       integer     not null check (frequency >= 1),
	primary key (recurring_event),
	foreign key (recurring_event) references Recurring_events(event_id)
);

create table Monthly_by_day_events (
	recurring_event integer,
	day_of_week     Day_of_week not null, -- same reason as for Weekly_events
	week_in_month   integer not null check (week_in_month >= 1 and week_in_month <= 5),
	primary key (recurring_event),
	foreign key (recurring_event) references Recurring_events(event_id)
);

create table Monthly_by_date_events (
	recurring_event integer,
	date_in_month   integer not null check (date_in_month >= 1 and date_in_month <= 31),
	primary key (recurring_event),
	foreign key (recurring_event) references Recurring_events(event_id)
);

/* I've chosen to store the date as the date type because we were only permitted
 * to use one field here and there doesn't seem to be a PSQL type that stores
 * just month and day. From the spec: "One thing you should not do is try to 
 * second-guess the requireents or the ER design, come up with your own ER, 
 * and implement that. This would be like taking some specs from your boss and 
 * implementing something that you thought was better than the specs.". The year 
 * part of this date stamp would just be ignored by the program using it. It may
 * even end up being helpful if it stores the year this annual event was started. */
create table Annual_events (
	recurring_event integer,
	date            date not null,
	primary key (recurring_event),
	foreign key (recurring_event) references Recurring_events(event_id)
);

-- Relationship tables

create table Member (
	user_id     integer,
	group_id    integer,
	primary key (user_id, group_id),
	foreign key (user_id) references Users(id),
	foreign key (group_id) references Groups(id)
);

/* The Accessibility_type enum covers all sensible values for accessibility. There
 * is no case where it is useful to have null here, and hence I have made it 
 * not null. The whole tuple would be useless if it were null as that's the only
 * useful piece of information stored on this relationship. */
create table Accessibility (
	user_id     integer,
	calendar_id integer,
	access      Accessibility_type not null default 'none', -- whole tuple is useless if this is null
	primary key (user_id, calendar_id),
	foreign key (user_id) references Users(id),
	foreign key (calendar_id) references Calendars(id)
);

create table Subscribed (
	user_id     integer,
	calendar_id integer,
	colour      text not null,
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
	status      Invite_status not null default 'invited', -- whole tuple is useless if this is null
	primary key (user_id, event_id),
	foreign key (user_id) references Users(id),
	foreign key (event_id) references Events(id)
);
