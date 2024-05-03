using System;
using System.IO;
using Microsoft.Data.Sqlite;

namespace HelloWorldSample
{
    class Program
    {
        static void Main()
        {
            using (var connection = new SqliteConnection("Data Source=pdr.db"))
            {
                connection.Open();

                var command = connection.CreateCommand();

                // Ask the User for A state code
                Console.Write("State Code: ");
                var code = Console.ReadLine();

                #region snippet_Parameter
                command.CommandText =
                @"
                  Select NAME FROM states WHERE code = $code
                ";
                command.Parameters.AddWithValue("$code", code);
                #endregion
                command.ExecuteNonQuery();

                var state = command.ExecuteScalar();
                Console.WriteLine($"Your State is {state}.");
            }

        }
    }
}
