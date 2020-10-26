--
--  This script creates the Beer database tables
--
--  Run it via:  \i schema.sql
--
--

create domain BeerName varchar(30);
create domain BarName varchar(30);
create domain DrinkerName varchar(20);

create table Beers (
	name    BarName,
	brewer  varchar(20),
	primary key (name)
);

create table Bars (
	name    BarName,
	addr    varchar(20),
	license integer,
	primary key (name)
);

create table Drinkers (
	name    DrinkerName,
	addr    varchar(30),
	phone   char(10),
	primary key (name)
);

create table Sells (
	bar     BarName,
	beer    BeerName,
	price   float,
	primary key (bar,beer),
	foreign key (bar) references Bars(name),
	foreign key (beer) references Beers(name)
);

create table Likes (
	drinker	DrinkerName,
	beer    BeerName,
	primary key (drinker,beer),
	foreign key (drinker) references Drinkers(name),
	foreign key (beer) references Beers(name)
);

create table Frequents (
	drinker DrinkerName,
	bar     BarName,
	primary key (drinker,bar),
	foreign key (drinker) references Drinkers(name),
	foreign key (bar) references Bars(name)
);
