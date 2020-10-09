create table Accounts (
    branchName text,
    accountNo char(4),
    balance integer
);

create table Customers (
    id integer,
    primary key (id)
)

create table Owner (
    account char(4),
    customer integer,
    primary key (account, customer),
    foreign key (account) references Accounts(accountNo),
    foreign key (customer) references Customers(id)
);