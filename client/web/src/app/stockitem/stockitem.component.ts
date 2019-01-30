import { Component, OnInit, Input } from '@angular/core';
import { IexService } from '../iex.service';
import { map, filter, switchMap } from 'rxjs/operators';
import {interval} from "rxjs/internal/observable/interval";

@Component({
  selector: 'app-stockitem',
  templateUrl: './stockitem.component.html',
  styleUrls: ['./stockitem.component.sass']
})
export class StockitemComponent implements OnInit {
  @Input("ticker") stockitem: string;
  price: string = "0";
  price_state: boolean;

  constructor(private iexservice: IexService) { }

  ngOnInit() {
    
    interval(1000).pipe(switchMap(() =>  this.iexservice.get_price(this.stockitem))).subscribe(price => {
      console.log(price + "====" + this.price);

      if(parseFloat(price) > parseFloat(this.price)){
        this.price_state = true;
        this.price = price;
      }

      if(parseFloat(price) < parseFloat(this.price)){
          this.price_state = false;
          this.price = price;
      }

    });

    
  }

}
