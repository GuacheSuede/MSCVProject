import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-analyze',
  templateUrl: './analyze.component.html',
  styleUrls: ['./analyze.component.sass']
})
export class AnalyzeComponent implements OnInit {

  constructor() { }

  ngOnInit() {
    eval(`window.rr = "rr";`)
  }


  
}
