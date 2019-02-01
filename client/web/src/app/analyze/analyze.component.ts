import { Component, OnInit } from '@angular/core';
import {CompileService} from "../compile.service";
import { SmoothieChart, TimeSeries }  from '../../assets/smoothie.js';
import '../../assets/object-watch.js';

@Component({
  selector: 'app-analyze',
  templateUrl: './analyze.component.html',
  styleUrls: ['./analyze.component.sass']
})
export class AnalyzeComponent implements OnInit {
  precompiled_code: string = "";
  compiled_code: string = "";
  options: Object;
  smoothie = new SmoothieChart({
    grid:{fillStyle:'#ffffff', fontSize:20},
    tooltip:true,
    labels:{fillStyle:'#000000', fontSize:20}
  });
  compiled = false;
  display_graph:boolean = true;

  constructor(private compileservice: CompileService) { }

  ngOnInit() {
    (<any>window).murl = "http://127.0.0.1:8080/db/prices?pagesize=1&sort_by=datetime&filter={\"ticker\": \"";
    (<any>window).url_end = "\"}";
    this.options = {
      folding: true,
      minimap: { enabled: false },
      readOnly: false,
      fontLigatures: true,
      fontFamily: "Courier New",
      fontSize: 18

    };
    (<any>window).TimeSeries = TimeSeries;
    (<any>window).smoothie = this.smoothie;
    (<any>window).random_rgba = function() {
      var o = Math.round, r = Math.random, s = 255;
      return 'rgba(' + o(r()*s) + ',' + o(r()*s) + ',' + o(r()*s) + ',' + 100 +  ')';
    }


  }

  compile_code(){
    if(this.compiled == false) {
      this.compileservice.compile(this.precompiled_code).subscribe(compiled_c => {

        this.compiled_code = compiled_c["code"];
        // console.log(this.compiled_code)
        (<any>window).codec = this.compiled_code;
        eval(this.compiled_code);
      });
      this.compiled = true;
    }
  }

  // display_graph(){
  //
  // }

  close_graph(){
    this.display_graph = false;
  }

  log(){
    console.log(this.precompiled_code)
  }

  
}
