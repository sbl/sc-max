{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 496.0, 187.0, 518.0, 591.0 ],
		"bglocked" : 0,
		"defrect" : [ 496.0, 187.0, 518.0, 591.0 ],
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
					"maxclass" : "comment",
					"text" : "doesn't work yet",
					"id" : "obj-15",
					"fontname" : "Verdana",
					"patching_rect" : [ 180.0, 120.0, 211.0, 36.0 ],
					"fontsize" : 24.0,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 0.337255, 0.619608, 1.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-4",
					"fontname" : "Verdana",
					"patching_rect" : [ 60.0, 190.0, 50.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-8",
					"fontname" : "Verdana",
					"patching_rect" : [ 100.0, 220.0, 50.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "in - input signal to be processed\nfreq - cutoff frequency.\nrq - the reciprocal of Q.  bandwidth / cutoffFreq. ",
					"linecount" : 3,
					"id" : "obj-16",
					"fontname" : "Verdana",
					"patching_rect" : [ 210.0, 240.0, 256.0, 43.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "A resonant low pass filter.",
					"id" : "obj-9",
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 40.0, 150.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"outlettype" : [ "" ],
					"scroll" : 2,
					"id" : "obj-14",
					"patching_rect" : [ 220.0, 370.0, 119.0, 91.0 ],
					"numinlets" : 2,
					"monochrome" : 0,
					"numoutlets" : 1,
					"sono" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"fontname" : "Verdana",
					"patching_rect" : [ 80.0, 430.0, 32.5, 17.0 ],
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"fontname" : "Verdana",
					"patching_rect" : [ 70.0, 410.0, 72.0, 17.0 ],
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~",
					"id" : "obj-6",
					"fontname" : "Verdana",
					"patching_rect" : [ 20.0, 460.0, 36.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.rlpf~ 300. 0.3",
					"outlettype" : [ "signal" ],
					"id" : "obj-5",
					"fontname" : "Verdana",
					"patching_rect" : [ 20.0, 265.0, 96.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 3,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "noise~",
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontname" : "Verdana",
					"patching_rect" : [ 20.0, 110.0, 45.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontname" : "Verdana",
					"patching_rect" : [ 20.0, 344.0, 45.0, 19.0 ],
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"id" : "obj-12",
					"fontname" : "Verdana",
					"patching_rect" : [ 5.0, 520.0, 279.0, 43.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.rlpf~",
					"id" : "obj-7",
					"fontname" : "Arial",
					"patching_rect" : [ 9.0, 11.0, 125.0, 25.0 ],
					"fontface" : 3,
					"fontsize" : 16.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"id" : "obj-10",
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-6", 1 ],
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
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 79.0, 315.0, 86.0, 323.0, 93.0, 340.0, 114.0, 340.0, 123.0, 321.0, 147.0, 298.0, 162.0, 287.0, 183.0, 307.0, 183.0, 317.0, 183.0, 326.0, 165.0, 326.0, 165.0, 318.0, 165.0, 310.0, 169.0, 310.0, 178.0, 310.0, 189.0, 310.0, 203.0, 310.0, 203.0, 326.0, 203.0, 360.0, 188.0, 360.0, 171.0, 360.0, 171.0, 349.0, 171.0, 339.0, 184.0, 339.0, 200.0, 339.0, 219.0, 351.0, 219.0, 365.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-5", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
