import { Component, OnInit } from '@angular/core';
import {CompileService} from "../compile.service";

@Component({
  selector: 'app-analyze',
  templateUrl: './analyze.component.html',
  styleUrls: ['./analyze.component.sass']
})
export class AnalyzeComponent implements OnInit {
  precompiled_code: string = "";
  compiled_code: string = "";
  constructor(private compileservice: CompileService) { }

  ngOnInit() {
    (<any>window).murl = "http://127.0.0.1:8080/db/prices?pagesize=1&sort_by=datetime&filter={\"ticker\": \"";
    (<any>window).url_end = "\"}";

    setInterval(()=>{
      this.precompiled_code = document.querySelector('textarea').value
    })
  }

  compile_code(){
    console.log(this.precompiled_code);
    this.compileservice.compile(this.precompiled_code).subscribe(compiled_c => {

      this.compiled_code = compiled_c["code"];
      // console.log(this.compiled_code)
      (<any>window).codec = this.compiled_code;
      eval(this.compiled_code);
    });
  }


  
}
