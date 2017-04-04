using System;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Deployment.WindowsInstaller;

namespace IndexerSetup.RegisterProperties
{
    public class RegisterAction
    {
        [CustomAction]
        public static ActionResult RegisterPropDescFile(Session session)
        {
            session.Log("Registering property description file...");

            string propdescPath = Path.Combine(session.GetTargetPath("INSTALLFOLDER"), "WordPerfectIndexer.propdesc");
            bool registerSucceeded = PSRegisterPropertySchema(propdescPath);
            if (!registerSucceeded) return ActionResult.Failure;

            SHChangeNotify(0x08000000, 0, IntPtr.Zero, IntPtr.Zero);
            return ActionResult.Success;
        }

        [DllImport("propsys.dll", CharSet = CharSet.Unicode)]
        public static extern bool PSRegisterPropertySchema(string propdescPath);

        [DllImport("shell32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern void SHChangeNotify(long eventId, uint flags, IntPtr data1, IntPtr data2);
    }
}
