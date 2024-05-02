using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CISS456
{
    internal class Class1
    {
        string dir_path = Environment.CurrentDirectory;
        string db_path = Path.Combine(dir_path, "docs/pdr.db");
    }
}
