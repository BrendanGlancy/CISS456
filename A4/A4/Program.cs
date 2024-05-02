using (var connection = new SqliteConnection)
{
    connection.Open();

    var command = connection.CreateCommand();
    command.CommandText = "";
           
}