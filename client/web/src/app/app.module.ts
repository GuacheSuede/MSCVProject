import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { HttpClientModule }    from '@angular/common/http';
import { AppComponent } from './app.component';

import { WatchlistsoverviewComponent } from './watchlistsoverview/watchlistsoverview.component';
import { WatchlistsselectionComponent } from './watchlistsselection/watchlistsselection.component';
import { WatchlistviewComponent } from './watchlistview/watchlistview.component';
import { WatchlistcreationComponent } from './watchlistcreation/watchlistcreation.component';
import { StockitemComponent } from './stockitem/stockitem.component';
import { AnalyzeComponent } from './analyze/analyze.component';

@NgModule({
  declarations: [
    AppComponent,
    WatchlistsoverviewComponent,
    WatchlistsselectionComponent,
    WatchlistviewComponent,
    WatchlistcreationComponent,
    StockitemComponent,
    AnalyzeComponent
    
    ,
    ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
