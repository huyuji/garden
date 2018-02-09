using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Mail;
using System.Xml;

//todo:
//body encoding
//parameter checking
//exception handling
//log send action
//support flexible iteration modes
//support regular expression in string formatting
//the use of reflection may further simplify the code structure


namespace sendmail
{
    public class MailConfig
    {
        public MailMessage Message = new MailMessage();
        public string Host;
        public string Password;
    }

    public class ConfigIterator : IEnumerable<MailConfig>
    {
        public List<string> From = new List<string>();

        public List<string> To = new List<string>();
        public List<string> Bcc = new List<string>();
        public List<string> Cc = new List<string>();

        public List<string> Subject = new List<string>();
        public List<string> Body = new List<string>();
        public List<string> Attachment = new List<string>();

        public List<string> Password = new List<string>();

        public List<string> Host = new List<string>();

        private readonly MailConfig _mailConfig = new MailConfig();

        public ConfigIterator(XmlNode configNode)
        {
            ReadStringList(configNode, "from", From);

            ReadStringList(configNode, "to", To);
            ReadStringList(configNode, "bcc", Bcc);
            ReadStringList(configNode, "cc", Cc);

            ReadStringList(configNode, "subject", Subject);
            ReadStringList(configNode, "body", Body);
            ReadStringList(configNode, "attachment", Attachment);

            ReadStringList(configNode, "password", Password);
            ReadStringList(configNode, "host", Host);
        }

        public IEnumerator<MailConfig> GetEnumerator()
        {
            _mailConfig.Password = Password[0];
            _mailConfig.Host = Host[0];

            foreach (string from in From)
            {
                _mailConfig.Message.From = new MailAddress(from);

                for (int i = 0; i < To.Count; i++)
                {
                    _mailConfig.Message.To.Add(new MailAddress(To[i]));

                    if(Bcc.Count > i)
                        _mailConfig.Message.Bcc.Add(new MailAddress(Bcc[i]));
    
                    if(Cc.Count > i)
                        _mailConfig.Message.CC.Add(new MailAddress(Cc[i]));

                    for (int j = 0; j < Subject.Count; j++)
                    {
                        _mailConfig.Message.Subject = Subject[j];
    
                        if(Body.Count > j)
                            _mailConfig.Message.Body = Body[j];

                        //if(Attachment.Count > j)
                        //    _mailConfig.Message.Attachments.Add(new Attachment(Attachment[j]));
    
                        yield return _mailConfig;
                    }

                    foreach (string attachment in Attachment)
                    {
                        _mailConfig.Message.Attachments.Add(new Attachment(attachment));
                    }
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private static void ReadStringList(XmlNode configNode, string elementName, List<string> list)
        {
            XmlNodeList nodeList = configNode.SelectNodes("descendant::" + elementName);

            if (nodeList == null)
                return;

            foreach (XmlNode node in nodeList)
            {
                string s = node.InnerText;
                if (s.IndexOf("(*)") == -1)
                    list.Add(s);
                else
                    list.AddRange(Parse(s));
            }
        }

        private static IEnumerable<string> Parse(string address)
        {
            List<string> addressList = new List<string>();
            address = address.Trim('\r', '\n', '\t', ' ');

            int wildIndex = address.IndexOf("(*)");
            if (wildIndex == -1)
                return addressList;

            string prefix = address.Substring(0, wildIndex);
            address = address.Substring(wildIndex + "(*)".Length + 1);
            int iterIndex = address.IndexOf(")");
            string iterString = address.Substring(0, iterIndex);
            string postfix = address.Substring(iterIndex + 1);
            int commaIndex = iterString.IndexOf(',');
            string format = iterString.Substring(0, commaIndex);
            iterString = iterString.Substring(commaIndex + 1);
            commaIndex = iterString.IndexOf(',');
            int start = Convert.ToInt32(iterString.Substring(0, commaIndex));
            int end = Convert.ToInt32(iterString.Substring(commaIndex + 1));

            for (int i = start; i <= end; i++)
            {
                string incrementor = string.Format(format, i);
                addressList.Add(prefix + incrementor + postfix);
            }

            return addressList;
        }
    }

    public class MailSender
    {
        private readonly XmlDocument _doc = new XmlDocument();
        private readonly List<ConfigIterator> _configIterators = new List<ConfigIterator>();

        public MailSender()
        {
            _doc.Load("sendmail.xml");
        }

        public MailSender(string configFilePath)
        {
            _doc.Load(configFilePath);
        }

        private void GetConfigs()
        {
            if(_doc.DocumentElement == null)
                return;

            XmlNode configNode = _doc.DocumentElement.FirstChild;

            while (configNode != null)
            {
                _configIterators.Add(new ConfigIterator(configNode));

                configNode = configNode.NextSibling;
            }
        }

        public static void Send(MailConfig mailConfig, SendCompletedEventHandler completedMethod)
        {
            SmtpClient smtpClient = new SmtpClient();
            smtpClient.Credentials = new NetworkCredential(mailConfig.Message.From.Address, mailConfig.Password);//设置发件人身份的票据  
            smtpClient.DeliveryMethod = SmtpDeliveryMethod.Network;
            smtpClient.Host = mailConfig.Host;

            if (completedMethod == null)
            {
                smtpClient.Send(mailConfig.Message);
            }
            else
            {
                smtpClient.SendCompleted += completedMethod;//注册异步发送邮件完成时的事件  
                smtpClient.SendAsync(mailConfig.Message, null);
            }
        }

        public void Send()
        {
            GetConfigs();
            foreach (ConfigIterator configIterator in _configIterators)
            {
                foreach (MailConfig mailConfig in configIterator)
                {
                    Send(mailConfig, null);
                }
            }
        }
    }

    class Program
    {

        static void Main(string[] args)
        {
            MailSender mailSender = new MailSender();
            mailSender.Send();
        }
    }
}
