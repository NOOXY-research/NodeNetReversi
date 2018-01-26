uuid = ""
havepass = 0
function guid() {
  return s4() + s4() + '-' + s4() + '-' + s4() + '-' +
    s4() + '-' + s4() + s4() + s4();
}

function s4() {
  return Math.floor((1 + Math.random()) * 0x10000)
    .toString(16)
    .substring(1);
}
function reversi_turn_red_em (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-red-em\"></div>";
}
function reversi_turn_red (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-red\"></div>";
}
function reversi_turn_blue (id) {
  x=document.getElementById(id);
  x.innerHTML = "<div class=\"reversi-map-blue\"></div>";
}
function reversi_set_tile (id, value) {
  if (value>0) {
    if(value>1) {
      value = 1
    }
    x=document.getElementById(id);
    x.innerHTML = "<div style=\"width: 100%; height:100%; background-color: rgba(244, 64, 131, "+value+")\"></div>";
  }
  else {
    if(value<-1) {
      value = -1
    }
    x=document.getElementById(id);
    x.innerHTML = "<div style=\"width: 100%; height:100%; background-color: rgba(66, 134, 244, "+(-value)+")\"></div>";
  }
}
function reversi_turn_none (id) {
  x=document.getElementById(id);
  x.innerHTML = " ";
}
function set_right_point (point) {
  x=document.getElementById("reversi-point-right");
  x.innerHTML ="<span class=\"reversi-point-anime\">"+ point+"</span><br><hr class=\"reversi-point-hr\" style=\"border-color: #f44183;\">";
}
function set_left_point (point) {
  x=document.getElementById("reversi-point-left");
  x.innerHTML ="<span class=\"reversi-point-anime\">"+ point+"</span><br><hr class=\"reversi-point-hr\" style=\"border-color: #00bcd4;\">";
}

function draw_value_map (key, debug) {
  console.log(debug);
  for (var x=0; x<8; x++) {
    for (var y=0; y<8; y++) {
      switch (key[x*8+y]) {
        case 'A':
          reversi_turn_blue("Valuer"+(y+1)+"c"+(x+1));
          break;
        case 'B':
          reversi_turn_red("Valuer"+(y+1)+"c"+(x+1));
          break;
        case 'N':
          reversi_turn_none("Valuer"+(y+1)+"c"+(x+1));
          break;
      }
    }
  }
  for (var x=0; x<8; x++) {
    for (var y=0; y<8; y++) {
      switch (key[x*8+y]) {
        case 'A':
          reversi_turn_blue("Policyr"+(y+1)+"c"+(x+1));
          break;
        case 'B':
          reversi_turn_red("Policyr"+(y+1)+"c"+(x+1));
          break;
        case 'N':
          reversi_turn_none("Policyr"+(y+1)+"c"+(x+1));
          break;
      }
    }
  }
  for (var x=0; x<8; x++) {
    for (var y=0; y<8; y++) {
      switch (key[x*8+y]) {
        case 'A':
          reversi_turn_blue("Sumr"+(y+1)+"c"+(x+1));
          break;
        case 'B':
          reversi_turn_red("Sumr"+(y+1)+"c"+(x+1));
          break;
        case 'N':
          reversi_turn_none("Sumr"+(y+1)+"c"+(x+1));
          break;
      }
    }
  }
  for(var x=0; x< debug.length; x++) {
    console.log(debug[x]);
    reversi_set_tile("Valuer"+((debug[x].DropPoint)[1]+1)+"c"+((debug[x].DropPoint)[0]+1), (debug[x].Value*2-1))
    reversi_set_tile("Policyr"+((debug[x].DropPoint)[1]+1)+"c"+((debug[x].DropPoint)[0]+1), (debug[x].Policy*2-1))
    reversi_set_tile("Sumr"+((debug[x].DropPoint)[1]+1)+"c"+((debug[x].DropPoint)[0]+1), (debug[x].Sum*2-1))
  }
}
function draw_map (key) {
  for (var x=0; x<8; x++) {
    for (var y=0; y<8; y++) {
      switch (key[x*8+y]) {
        case 'A':
          reversi_turn_blue("r"+(y+1)+"c"+(x+1));
          break;
        case 'B':
          reversi_turn_red("r"+(y+1)+"c"+(x+1));
          break;
        case 'N':
          reversi_turn_none("r"+(y+1)+"c"+(x+1));
          break;
      }
    }
  }
}
function drop (x, y) {
  havepass = 1;
  coverer=document.getElementById("coverer");
  coverer.classList.remove("style-hidden");
  coverer_loader=document.getElementById("coverer-loader");
  coverer_loader.classList.remove("style-hidden");
  var get = new XMLHttpRequest();
  get.open('GET', "/reversi/response_ann?key="+mykey+"&x="+(x-1)+"&y="+(y-1)+"&uuid="+uuid, true);
  get.send();
  get.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
       var response = JSON.parse(this.responseText);
       if (response.message == "err")
       {
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 10);
         return 0;
       }else {
         mykey=response.key1;
         set_left_point(response.p1_1);
         set_right_point(response.p2_1);
         draw_map(mykey);
         draw_value_map(mykey, response.debug);
         mykey=response.key2;
         setTimeout(function() {
           set_left_point(response.p1_2);
           set_right_point(response.p2_2);
           draw_map(mykey);
           if (response.x!=-1) {
             reversi_turn_red_em("r"+(parseInt(response.y)+1)+"c"+(parseInt(response.x)+1));
           }
         }, 500);
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
       }
    }
    else if(this.readyState == 4) {
      setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
    }
  }
  draw_map(mykey);

}
function pass () {
  if(havepass) {
    return 0;
  }
  else {
    havepass = 1;
  }
  coverer=document.getElementById("coverer");
  coverer.classList.remove("style-hidden");
  coverer_loader=document.getElementById("coverer-loader");
  coverer_loader.classList.remove("style-hidden");
  var get = new XMLHttpRequest();
  get.open('GET', "/reversi/response_ann?key="+mykey+"&x=-1&y=-1&uuid="+uuid+"&pass=1", true);
  get.send();
  get.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
       var response = JSON.parse(this.responseText);
       if (response.message == "err")
       {
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
         return 0;
       }else {
         mykey=response.key1;
         set_left_point(response.p1_1);
         set_right_point(response.p2_1);
         draw_map(mykey);
         mykey=response.key2;
         setTimeout(function() {
           set_left_point(response.p1_2);
           set_right_point(response.p2_2);
           draw_map(mykey);
           if (response.x!=-1) {
             reversi_turn_red_em("r"+(parseInt(response.y)+1)+"c"+(parseInt(response.x)+1));
           }
         }, 1000);
         setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 1000);
       }
    }
    else if(this.readyState == 4) {
      setTimeout(function(){coverer.classList.add("style-hidden");coverer_loader.classList.add("style-hidden");}, 500);
    }
  }
  draw_map(mykey);

}
var mykey="NNNNNNNNNNNNNNNNNNNNNNNNNNNBANNNNNNABNNNNNNNNNNNNNNNNNNNNNNNNNNN";
//initailize
function initailize () {
  uuid = guid();
  console.log('UUID = '+uuid)
  draw_map (mykey);
  set_right_point(2);
  set_left_point(2);
  coverer=document.getElementById("coverer");
  coverer_loader=document.getElementById("coverer-loader");
  coverer.classList.add("style-hidden");
  coverer_loader.classList.add("style-hidden");
  // pass();
}
