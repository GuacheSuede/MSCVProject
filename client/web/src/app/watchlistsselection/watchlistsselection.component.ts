import { Component, OnInit } from '@angular/core';
import { watchlists } from '../mock-watchlist';
import { WatchlistService } from '../watchlist.service'
import { watchlist } from '../watchlist'

@Component({
  selector: 'app-watchlistsselection',
  templateUrl: './watchlistsselection.component.html',
  styleUrls: ['./watchlistsselection.component.sass']
})
export class WatchlistsselectionComponent implements OnInit {
  // watchlistsdata = watchlists;
  watchlistsdata: watchlist[];
  selectedwatchlist: string[];

  constructor(private watchlistservice: WatchlistService) { }

  ngOnInit() {
  	this.get_watchlists();
  }

  get_watchlists(): void {
  	this.watchlistservice.get_watchlists()
  	.subscribe( watchlists => this.watchlistsdata = watchlists );
  }

  select_watchlist(name: string): void{
    this.selectedwatchlist = this.watchlistsdata.filter( wl => {
      return wl.name == name;
    })[0].stocks;
    console.log(this.selectedwatchlist)
  }

}

//just subling for now, we are shipping, fuck learning new
