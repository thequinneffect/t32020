create table Accounts (
    branchName text,
    accountNo char(5),
    balance integer,
    primary key (accountNo)
);

create table Customers (
    id integer,
    primary key (id)
);

create table Owner (
    account char(5),
    customer integer,
    primary key (account, customer),
    foreign key (account) references Accounts(accountNo),
    foreign key (customer) references Customers(id)
);