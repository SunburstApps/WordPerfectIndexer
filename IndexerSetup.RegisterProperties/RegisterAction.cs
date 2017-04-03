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

            string propdescPath = Path.Combine(session.GetTargetPath("INSTALLDIR"), "WordPerfectIndexer.propdesc");
            bool registerSucceeded = PSRegisterPropertySchema(propdescPath);

            if (registerSucceeded) return ActionResult.Success;
            else return ActionResult.Failure;
        }

        [CustomAction]
        public static ActionResult UnregisterPropDescFile(Session session)
        {
            session.Log("Unregistering property description file...");

            string propdescPath = Path.Combine(session.GetTargetPath("INSTALLDIR"), "WordPerfectIndexer.propdesc");
            bool registerSucceeded = PSUnregisterPropertySchema(propdescPath);

            if (registerSucceeded) return ActionResult.Success;
            else return ActionResult.Failure;
        }

        [DllImport("propsys.dll", CharSet = CharSet.Unicode)]
        public static extern bool PSRegisterPropertySchema(string propdescPath);
        [DllImport("propsys.dll", CharSet = CharSet.Unicode)]
        public static extern bool PSUnregisterPropertySchema(string propdescPath);
    }
}
