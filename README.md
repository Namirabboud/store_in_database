# store_in_database
store informations like usernames and passwords from text file in sqlite database.

Having a text file containing usernames and passwords that u wish to store<br>
in sqlite database.<br>

1- First information in text file must have a specific layout<br>

    ===>"account name"
    username: "username"
    password: "password"
    
    ===> "account name2"
    username: "username2"
    password: "password2"
    ...
    
2- Compile the source code with sqlite3 lib<br>
    gcc -o store store.c -lsqlite3<br>

3- Enter the name of the file that containes the informations as an argument<br>
    ./store 'full path/file name'<br>
    
4- When running the script first u will be asked for the string that defines the account name<br>
    in the example above "===>" is what define the account name.<br>

5- Then you will be asked for the string that define the username<br>
    in the example above "username:" is what define the username.<br>
    
6- And "password:" is what define the password.<br>

7- Finaly full path and database name then will be asked for.

Enjoy!
