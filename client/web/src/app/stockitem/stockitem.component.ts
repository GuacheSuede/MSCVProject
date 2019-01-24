import { Component, OnInit, Input } from '@angular/core';
import { IexService } from '../iex.service';

@Component({
  selector: 'app-stockitem',
  templateUrl: './stockitem.component.html',
  styleUrls: ['./stockitem.component.sass']
})
export class StockitemComponent implements OnInit {
  @Input("ticker") stockitem: string;
  price: string;

  constructor(private iexservice: IexService) { }

  ngOnInit() {
    this.iexservice.get_price(this.stockitem).subscribe(price => {this.price = price; });
  }

}
