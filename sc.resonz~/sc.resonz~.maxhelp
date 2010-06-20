{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 443.0, 120.0, 715.0, 590.0 ],
		"bglocked" : 0,
		"defrect" : [ 443.0, 120.0, 715.0, 590.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 10.0, 10.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"id" : "obj-2",
					"numinlets" : 1,
					"patching_rect" : [ 110.0, 220.0, 50.0, 19.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.5",
					"fontsize" : 10.0,
					"id" : "obj-24",
					"numinlets" : 2,
					"patching_rect" : [ 10.0, 130.0, 45.0, 19.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "in - input signal to be processed\nfreq - resonant frequency in Hertz\nrq - bandwidth ratio (reciprocal of Q). rq = bandwidth / centerFreq\nThe reciprocal of Q is used rather than Q because it saves a divide operation inside the unit generator.",
					"linecount" : 5,
					"fontsize" : 10.0,
					"id" : "obj-20",
					"numinlets" : 1,
					"patching_rect" : [ 300.0, 240.0, 352.0, 67.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "A two pole resonant filter with zeroes at z = +/- 1. Based on K. Steiglitz,  \"A Note on Constant-Gain \nDigital Resonators,\" Computer Music Journal, vol 18, no. 4, pp. 8-10, Winter 1994.\n",
					"linecount" : 2,
					"fontsize" : 10.0,
					"id" : "obj-18",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 50.0, 523.0, 31.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arg freq, rq",
					"fontsize" : 10.0,
					"id" : "obj-16",
					"numinlets" : 1,
					"patching_rect" : [ 140.0, 250.0, 68.0, 19.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~ 1",
					"fontsize" : 10.0,
					"id" : "obj-9",
					"numinlets" : 2,
					"patching_rect" : [ 60.0, 160.0, 54.0, 19.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.range~ @low 100 @high 5000",
					"fontsize" : 10.0,
					"id" : "obj-8",
					"numinlets" : 1,
					"patching_rect" : [ 60.0, 190.0, 179.0, 19.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"id" : "obj-14",
					"numinlets" : 2,
					"monochrome" : 0,
					"scroll" : 2,
					"patching_rect" : [ 380.0, 420.0, 119.0, 91.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"sono" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"fontsize" : 10.0,
					"id" : "obj-13",
					"numinlets" : 2,
					"patching_rect" : [ 70.0, 420.0, 32.5, 17.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"fontsize" : 10.0,
					"id" : "obj-11",
					"numinlets" : 2,
					"patching_rect" : [ 60.0, 400.0, 72.0, 17.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~",
					"fontsize" : 10.0,
					"id" : "obj-6",
					"numinlets" : 2,
					"patching_rect" : [ 10.0, 450.0, 36.0, 19.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.resonz~ 1000 0.1",
					"fontsize" : 10.0,
					"id" : "obj-5",
					"numinlets" : 3,
					"patching_rect" : [ 10.0, 255.0, 119.0, 19.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.whitenoise~",
					"fontsize" : 10.0,
					"id" : "obj-3",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 100.0, 86.0, 19.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontsize" : 10.0,
					"id" : "obj-12",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 520.0, 279.0, 43.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.resonz~",
					"fontface" : 3,
					"fontsize" : 16.0,
					"id" : "obj-7",
					"numinlets" : 1,
					"patching_rect" : [ 9.0, 11.0, 125.0, 25.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"id" : "obj-10",
					"numinlets" : 1,
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"numoutlets" : 0,
					"rounded" : 0,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-5", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 28.0, 298.0, 54.0, 291.0, 46.0, 313.0, 80.0, 313.0, 55.0, 398.0, 91.0, 367.0, 123.0, 349.0, 138.0, 381.0, 151.0, 413.0, 200.0, 372.0, 200.0, 407.0, 200.0, 445.0, 229.0, 445.0, 229.0, 402.0, 269.0, 389.0, 269.0, 421.0, 269.0, 444.0, 290.0, 451.0, 309.0, 434.0, 329.0, 395.0, 350.0, 395.0, 339.0, 456.0, 369.0, 415.0, 388.0, 379.0, 420.0, 379.0, 464.0, 401.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 19.5, 406.0, 36.5, 406.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
