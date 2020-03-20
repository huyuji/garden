using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.Coverage.Analysis;
using System.Xml;
using System.Text.RegularExpressions;
using System.Xml.Xsl;

// http://codetuner.blogspot.ca/2011/09/convert-mstest-code-covarage-results-in.html

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


            XslTransform myXslTransform = new XslTransform();
            XmlTextWriter writer = new XmlTextWriter(wspath + @"coverage.htm", null);
            myXslTransform.Load(wspath + @"\style.xslt");
            myXslTransform.Transform(xmlDocument, null, writer);

            writer.Flush();
            writer.Close();
        }
    }
}
