# Postup 

Příkaz z Package Console:

```bash
Add-Migration "Complaints"
```

Výstup:
```bash
Build started...
Build succeeded.
The name 'Complaints' is used by an existing migration.
```




## ComplaintEntity

```cs
using Microsoft.AspNetCore.Http;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Threading.Tasks;
using UTB.Eshop.Web.Models.Validations;

namespace UTB.Eshop.Web.Models.Entity
{
    [Table(nameof(Complaint))]
    public class Complaint
    {
        [Key]
        [Required]
        public int ID { get; set; }

        [StringLength(255)]
        [Required]
        public string ProductID { get; set; }

        [StringLength(50)]
        [Required]
        public string Reason { get; set; }
    }
}

```


## EshopDbContext
```cs
public class EshopDbContext : IdentityDbContext<User, Role, int>
    {
        public DbSet<CarouselItem> CarouselItems { get; set; }
        public DbSet<Product> Products { get; set; }
        public DbSet<Order> Orders { get; set; }
        public DbSet<OrderItem> OrderItems { get; set; }
        
        // Přidáno zde:
        public DbSet<Complaint> Complaints { get; set; }

        public EshopDbContext(DbContextOptions options) : base(options)
        {

        }
 
 //...
 ```


## ComplaintController.cs
```cs
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Authorization;
using UTB.Eshop.Web.Models.Database;
using UTB.Eshop.Web.Models.Entity;
using UTB.Eshop.Web.Models.Identity;

namespace UTB.Eshop.Web.Areas.Customer.Controllers
{

    [Area("Customer")]
    [Authorize(Roles = nameof(Roles.Customer))]
    public class ComplaintController : Controller
    {
        // GET: /<controller>/
        public IActionResult Index()
        {
            return View();
        }

        readonly EshopDbContext eshopDbContext;
      

        public ComplaintController(EshopDbContext eshopDB)
        {
            eshopDbContext = eshopDB;
        }

        [HttpGet]
        public async Task<IActionResult> ComplainIt() {
            System.Diagnostics.Debug.WriteLine("COMPLAIN IT");

            var complaint = new Complaint();
            complaint.ID = 1;
            complaint.ProductID = "A1";
            complaint.Reason = "Not a good size";

            eshopDbContext.Complaints.Add(complaint);
            await eshopDbContext.SaveChangesAsync();

            return View(complaint);
        }

        public IActionResult Complain()
        {
            return View();
        }
    }
}
```






## index.cshtml

Zkoušel jsem oba způsoby volání funkce a controlleru
```html
@using UTB.Eshop.Web.Areas.Customer.Controllers
//...

  <a asp-controller="ComplaintController" asp-action="ComplainIt" asp-route-ID="@Model.Products[i].ID">
                                            <h2>Click me</h2>
                                        </a>                                     
```

```html
@using UTB.Eshop.Web.Areas.Customer.Controllers

//...
 <a class="btn btn-outline-dark mt-auto"
    onclick="Buy(@Model.Products[i].ID, '@Url.Action("ComplainIt", "ComplaintController", new { Area = "Customer" })',
    '#total_price', '@System.Globalization.CultureInfo.CurrentCulture.Name')">Complaint</a>
```
                                        
                                        
                                        
