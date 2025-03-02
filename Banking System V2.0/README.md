# Banking System V2.0

A **C++** command-line banking application that demonstrates **user registration**, **login**, **fund transfers**, **advanced account actions**, and **logging**. This project aims to practice concepts such as **file handling**, **OOP design**, **encapsulation**, and **basic encryption** of user passwords.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [User Roles and Secret Passcodes](#user-roles-and-secret-passcodes)
- [File Structure & Organization](#file-structure--organization)
- [Compile & Run](#compile--run)
- [How to Use](#how-to-use)
- [Implementation Details](#implementation-details)
- [Future Improvements](#future-improvements)
- [Contributing](#contributing)

---

## Overview

**Banking System V2.0** simulates a basic banking environment:
- Multiple users can be **registered** with unique usernames, passwords, and generated card numbers.
- Each user can **log in** securely, manage their funds, **transfer money** to others, and **view a log** of their operations.
- An **Admin** user has elevated powers, such as adding/removing money from any account or even **deleting the entire database** (except the logs) after a special password confirmation.
- The system is structured into **classes** (e.g., `User`, `DatabaseData`, `DatabaseMeta`, `LogService`) to separate concerns and practice OOP.

---

## Features

1. **User Registration**  
   - Provide a **username** and **password**.  
   - The system generates a **card number**.  
   - The password is optionally **encoded** (or lightly encrypted) for storage in `database.txt`.

2. **Login & Authentication**  
   - Enter username and password.  
   - If the account is **blocked**, access is denied.  
   - Up to three incorrect password attempts block the account automatically.

3. **Account Actions**  
   - **Check Balance**: Display your current balance.  
   - **Deposit Money**: Increase your balance.  
   - **Withdraw Money**: Decrease your balance (with possible fees if you have certain statuses).  
   - **Transfer Money**: Send funds to another user by **username**.  
   - **Change Password**: Securely update your password.  
   - **Block Account**: Block your own account if you wish to lock yourself out.

4. **User Statuses & Fees**  
   - **ARCA** user: 1% withdrawal/transfer fee.  
   - **VISA** user: 0.5% withdrawal/transfer fee.  
   - **PREMIUM** user: 0% fee.  
   - **ACTIVE** user: 0% fee (no special status).  
   - **ADMIN** user: 0% fee, plus elevated privileges (see below).

5. **Admin Privileges**  
   - **Add/Remove Funds** from any user’s account.  
   - **View or Unblock** any user’s account.  
   - **Delete the Database** (files `database.txt` and `metainfo.txt`), leaving only `log.txt` intact, after confirming with a special admin password.  
   - **Secret passcode** to become Admin: `admin777` (see below).

6. **Logging**  
   - All actions (login, deposit, withdrawal, transfer, admin changes) are appended to **`log.txt`**.  
   - You can see your personal transaction history (`User::showLog(...)`) or the admin can see all logs.

---

## User Roles and Secret Passcodes

- **Regular User** (`Status::ACTIVE`): The default status upon registration.  
- **Arca / Visa / Premium**:  
  - Chosen during registration or set by an admin. Each has different fees for withdrawal/transfers.  
- **Admin** (`Status::ADMIN`):  
  - Top-level privileges. Can forcibly add or remove money, block/unblock accounts, delete the database, etc.  
  - **Secret code** to become Admin (if allowed in the menu): `admin777`.  
- **Blocked** (`Status::BLOCKED`):  
  - A blocked user cannot log in until unblocked by an admin (or the system if allowed).  
- **Additional passcodes**:  
  - If you had a moderator role, it might have been `mod123`, but you mentioned removing the moderator role. Adjust or omit as necessary.

---

## File Structure & Organization

```
BankingSystemV2.0/
├─ include/
│  ├─ AccountMembers.hpp
│  ├─ DatabaseData.hpp
│  ├─ DatabaseMeta.hpp
│  ├─ EnumClassStatus.hpp
│  ├─ LogService.hpp
│  ├─ User.hpp
│  └─ Utility.hpp
├─ src/
│  ├─ Account.cpp
│  ├─ BankApplication.cpp
│  ├─ CubeEncoder.cpp
│  ├─ DatabaseData.cpp
│  ├─ DatabaseMeta.cpp
│  ├─ LogService.cpp
│  ├─ User.cpp
│  └─ Utility.cpp
│  ├─ EnumClassStatus.cpp
│  ├─ Registration.cpp
│  ├─ Main.cpp
│  ├─ LogService.cpp
│  ├─ Login.cpp
├─ Makefile
├─ README.md
└─ (the compiled executable, e.g. `BankingApp`)
```

- **`include/`**: All `.hpp` headers.  
- **`src/`**: Corresponding `.cpp` implementations.  
- **`Main.cpp`**: Entry point for the banking application.  
- **`Makefile`**: For building the project (adjust as needed).  

---

## Compile & Run

1. **Clone** the repository:
   ```bash
   git clone https://github.com/kharaazyan/Projects/tree/main/Banking%20System%20V2.0
   cd BankingSystemV2.0
   ```
2. **Build** using the Makefile:
   ```bash
   make
   ```
   This compiles the sources into an executable (e.g., `BankingApp`).

3. **Execute**:
   ```bash
   ./BankingApp
   ```
   or
   ```bash
   make run
   ```

---

## How to Use

1. **Launch the program**  
   The main menu appears:
   ```
   1) Register
   2) Log in
   3) Exit
   ```
2. **Register**  
   - Enter a username (up to 32 chars) and password (8–25 chars).  
   - The system assigns a card number automatically.  
3. **Log In**  
   - Provide username and password.  
   - If the user is blocked, you’ll be denied access.  
4. **Main User Menu**  
   - **Check Balance**  
   - **Deposit/Withdraw** (with possible fees if ARCA or VISA).  
   - **Transfer** to another user by username.  
   - **Change Password**  
   - **Block Account** (self-block)  
   - **Become Admin**: Enter the secret code `admin777` if you want the top privileges.  
   - **Fees**: If your status is `ARCA`, there’s a 1% fee on withdraw/transfer. If `VISA`, 0.5%. If `PREMIUM`, 0%.  
5. **Admin Menu**  
   - **Add/Remove money** from any user’s account.  
   - **View or Unblock** any user’s account.  
   - **Delete Database** (except `log.txt`) after confirming with the admin password `root123` (or whichever you configured).  
   - **Logs**: See a list of all transactions or filter them if you want to customize.  

---

## Implementation Details

- **OOP Classes**:
  - **`User`**: Implements user logic (login, deposit, withdraw, transfer, show log, block, etc.).  
  - **`DatabaseData`**: Handles reading and writing the main `database.txt`.  
  - **`DatabaseMeta`**: Stores statistics (number of blocked users, total money, number of transactions, etc.) in `metainfo.txt`.  
  - **`LogService`**: Manages logging of operations to `log.txt`.  
  - **`EnumClassStatus`**: Defines statuses (ACTIVE, ARCA, VISA, PREMIUM, ADMIN, BLOCKED, etc.).  

- **File I/O**:
  - **`database.txt`**: Each line represents a user’s data (username, password, card number, balance, user status, etc.).  
  - **`metainfo.txt`**: Aggregated stats: number of accounts, total money, blocked count, admin count, etc.  
  - **`log.txt`**: Each operation (login, deposit, withdraw, transfer, block) appended as a string.  

- **Password Encoding**:
  - Basic **encode/decode** (e.g., with a cube rotation algorithm or simple shifting).  
  - The user’s password is stored in an encoded form for demonstration, not intended for real security.  

- **Fees**:
  - Implemented by `getFeePercent()`:  
    - Arca = 0.01, Visa = 0.005, Premium = 0.0, others = 0.0.  
  - If the user doesn’t have enough funds to cover `amount + fee`, the operation fails.

- **Blocking**:
  - 3 wrong password attempts blocks the user.  
  - A user can also self-block.  
  - Admin can unblock.  

- **Deleting Database**:
  - Admin must enter the admin password (e.g. `"root123"`) to confirm.  
  - The system removes `database.txt` and `metainfo.txt`, leaving `log.txt` as a record.  

---

Enjoy experimenting with your own **Banking System V2.0**!
