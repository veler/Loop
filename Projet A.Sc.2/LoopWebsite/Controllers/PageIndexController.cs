using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.Mail;
using System.Net.Sockets;
using System.Text;
using System.Web.Mvc;
using LoopWebsite.Attributs;
using LoopWebsite.Models;
using Newtonsoft.Json;

namespace LoopWebsite.Controllers
{
    public class PageIndexController : Controller
    {
        #region Fields

        public static PageModel PageModelStatic = new PageModel();
        public static PhotoModel PhotoModelStatic = new PhotoModel();

        #endregion

        #region Methods

        private string CallCrawlAndSearchApp(string value)
        {
            string result = string.Empty;
            try
            {
                var buffer = new byte[104857600]; // 104857600 = 100MByte
                var socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Connect("127.0.0.1", 11000);
                socket.ReceiveBufferSize = buffer.Length;

                var query = Encoding.ASCII.GetBytes(value);
                socket.Send(query);

                var bytesRec = socket.Receive(buffer);
                result = Encoding.ASCII.GetString(buffer, 0, bytesRec);
                result = ClearText(result);

                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }
            catch (ArgumentNullException ex)
            {
                Console.WriteLine("ArgumentNullException : {0}", ex);
            }
            catch (SocketException ex)
            {
                Console.WriteLine("SocketException : {0}", ex);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Unexpected exception : {0}", ex);
            }
            return result;
        }

        private string ClearText(string text)
        {
            text = text.Replace("&agrave;", "à");
            text = text.Replace("&Agrave;", "À");

            text = text.Replace("&acirc;", "â");
            text = text.Replace("&Acirc;", "Â");

            text = text.Replace("&ccedil;", "ç");
            text = text.Replace("&Ccedil;", "Ç");

            text = text.Replace("&eacute;", "é");
            text = text.Replace("&Eacute;", "É");

            text = text.Replace("&egrave;", "è");
            text = text.Replace("&Egrave;", "È");

            text = text.Replace("&ecirc;", "ê");
            text = text.Replace("&Ecirc;", "Ê");

            text = text.Replace("&icirc;", "î");
            text = text.Replace("&Icirc;", "Î");

            text = text.Replace("&iuml;", "ï");
            text = text.Replace("&Iuml;", "Ï");

            text = text.Replace("&ocirc;", "ô");
            text = text.Replace("&Ocirc;", "Ô");

            text = text.Replace("&ugrave;", "ù");
            text = text.Replace("&Ugrave;", "Ù");
            return text;
        }

        #endregion

        #region ActionResult

        public ActionResult Home()
        {
            return View();
        }

        public ActionResult Privacy()
        {
            return View();
        }

        public ActionResult Terms()
        {
            return View();
        }

        public ActionResult About()
        {
            return View();
        }

        public ActionResult Submit(PageModel model)
        {
            var result = CallCrawlAndSearchApp("{C}|" + model.Query);
            return View(model);
        }

        [HttpPost]
        [MultipleButton(Name = "action", Argument = "Search")]
        public ActionResult Search(int pageIndex, PageModel model)
        {
            var result = CallCrawlAndSearchApp("{T}|" + model.Query);
            model.Pages = JsonConvert.DeserializeObject<PageModel>(result).Pages;
            PageModelStatic = model;
            return View(model);
        }

        public ActionResult NextPage(int pageIndex, int numberOfPage = 10)
        {
            var model = PageModelStatic;
            model.ActualPageIndex = pageIndex;
            model.NumberOfPageToDisplay = numberOfPage;
            return View(model);
        }

        [HttpPost]
        [MultipleButton(Name = "action", Argument = "SearchImages")]
        public ActionResult SearchImages(PhotoModel model)
        {
            var result = CallCrawlAndSearchApp("{I}|" + model.Query + "|" + model.Color + "|" + model.Size);
            model.Photos = JsonConvert.DeserializeObject<PhotoModel>(result).Photos;
            PhotoModelStatic = model;
            return View(model);
        }

        public ActionResult NextPhoto(int pageIndex, int numberOfPage = 10)
        {
            var model = PhotoModelStatic;
            model.ActualPageIndex = pageIndex;
            return View(model);
        }

        public ActionResult SearchImages(string color, string size, string query, PhotoModel model)
        {
            var result = CallCrawlAndSearchApp("{I}|" + query + "|" + color + "|" + size);
            model.Photos = JsonConvert.DeserializeObject<PhotoModel>(result).Photos;
            model.Color = color;
            model.Size = size;
            model.Query = query;
            PhotoModelStatic = model;
            return View(model);
        }

        public ActionResult SharePage(string title, string url, PageModel model)
        {
            //title = string.IsNullOrEmpty(title) ? "Link from Loop Search" : title;
            //var myProcess = new Process();
            //myProcess.StartInfo.FileName = "mailto:target@domain.com?subject=" + title + "&body=Hi ! I have a link for you, from Loop Search ! " + url;
            //myProcess.StartInfo.UseShellExecute = true;
            //myProcess.StartInfo.RedirectStandardOutput = false;
            //myProcess.Start();
            
            

            CallCrawlAndSearchApp("{S}|" + url);
            PageModelStatic.Pages.First(p => p.Url == url).ShareCount++;
            return RedirectToAction("NextPage", "PageIndex", new { pageIndex = PageModelStatic.ActualPageIndex });
        }

        public ActionResult LikePage(string url, PageModel model)
        {
            CallCrawlAndSearchApp("{L}|" + url);
            PageModelStatic.Pages.First(p => p.Url == url).LikeCount++;
            return RedirectToAction("NextPage", "PageIndex", new { pageIndex = PageModelStatic.ActualPageIndex });
        }

        public ActionResult ViewPage(string url, PageModel model)
        {
            CallCrawlAndSearchApp("{V}|" + url);
            PageModelStatic.Pages.First(p => p.Url == url).ViewCount++;
            return Redirect(url);
        }

        public ActionResult SharePhoto(string title, string url, PhotoModel model)
        {
            title = string.IsNullOrEmpty(title) ? "Picture from Loop Search" : title;
            var myProcess = new Process();
            myProcess.StartInfo.FileName = "mailto:target@domain.com?subject=" + title + "&body=Hi ! I have a picture for you, from Loop Search ! " + url;
            myProcess.StartInfo.UseShellExecute = true;
            myProcess.StartInfo.RedirectStandardOutput = false;
            myProcess.Start();

            CallCrawlAndSearchApp("{W}|" + url);
            PhotoModelStatic.Photos.First(p => p.Url == url).ShareCount++;
            return RedirectToAction("NextPhoto", "PageIndex", new { pageIndex = PhotoModelStatic.ActualPageIndex });
        }

        public ActionResult LikePhoto(string url, PhotoModel model)
        {
            CallCrawlAndSearchApp("{X}|" + url);
            PhotoModelStatic.Photos.First(p => p.Url == url).LikeCount++;
            return RedirectToAction("NextPhoto", "PageIndex", new { pageIndex = PhotoModelStatic.ActualPageIndex });
        }

        public ActionResult ViewPhoto(string url, PhotoModel model)
        {
            CallCrawlAndSearchApp("{Y}|" + url);
            PhotoModelStatic.Photos.First(p => p.Url == url).ViewCount++;
            return Redirect(url);
        }
        #endregion

    }
}