import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-analyze',
  templateUrl: './analyze.component.html',
  styleUrls: ['./analyze.component.sass']
})
export class AnalyzeComponent implements OnInit {

  constructor() { }

  ngOnInit() {
    (<any>window).murl = "http://127.0.0.1:8080/db/prices?pagesize=1&sort_by=datetime&filter={\"ticker\": \"";
    (<any>window).url_end = "\"}";
  }


  
}
