using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.Coverage.Analysis;
using System.Xml;
using System.Text.RegularExpressions;
using System.Xml.Xsl;

namespace coverage3
{
    class Program
    {
        static void Main(string[] args)
        {
            string wspath = Environment.CurrentDirectory.ToString();

            Regex isString = new Regex("(['\"]?)[a-zA-Z]*\\1$");
            CoverageInfo coverage = CoverageInfo.CreateFromFile(args[0]);

            CoverageDS data = coverage.BuildDataSet(null);


            string xml = data.GetXml();


            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.LoadXml(xml);
            xmlDocument.Save("coverage.xml");
        }
    }
}
