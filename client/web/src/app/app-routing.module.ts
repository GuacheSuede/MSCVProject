import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { WatchlistsoverviewComponent } from './watchlistsoverview/watchlistsoverview.component';
import { WatchlistcreationComponent } from './watchlistcreation/watchlistcreation.component';

const routes: Routes = [
    {path: 'watch', component: WatchlistsoverviewComponent},
    {path: 'watch_creation', component: WatchlistcreationComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
