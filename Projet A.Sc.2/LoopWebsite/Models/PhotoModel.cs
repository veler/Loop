using System.Collections.Generic;

namespace LoopWebsite.Models
{
    public class PhotoModel
    {
        public PhotoModel()
        {
            NumberOfPhotosToDisplay = 10;
            ActualPageIndex = 0;
            Size = "All";
            Color = "All";
        }

        public int Id { get; set; }
        public List<Photo> Photos { get; set; }
        public string Query { get; set; }
        public int NumberOfPhotosToDisplay { get; set; }
        public string Size { get; set; }
        public string Color { get; set; }
        public int ActualPageIndex { get; set; }
    }
}