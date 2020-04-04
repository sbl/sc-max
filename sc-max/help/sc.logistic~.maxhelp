{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 122.0, 129.0, 938.0, 690.0 ],
		"bglocked" : 0,
		"defrect" : [ 122.0, 129.0, 938.0, 690.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
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
					"text" : "dcblock",
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 201.0, 247.0, 52.0, 19.0 ],
					"id" : "obj-20",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "freq",
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 371.0, 138.0, 82.0, 19.0 ],
					"id" : "obj-15",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "chaos",
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 259.0, 142.0, 82.0, 19.0 ],
					"id" : "obj-14",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p mouse_coords",
					"fontname" : "Verdana",
					"numoutlets" : 2,
					"patching_rect" : [ 251.0, 118.0, 128.0, 19.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-38",
					"fontsize" : 10.0,
					"numinlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 0.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 900 10. 10000. 1.01",
									"fontname" : "Verdana",
									"numoutlets" : 1,
									"patching_rect" : [ 280.0, 247.0, 153.0, 19.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-27",
									"fontsize" : 10.0,
									"numinlets" : 6
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0 0 0 0",
									"fontname" : "Verdana",
									"numoutlets" : 4,
									"patching_rect" : [ 267.0, 185.0, 86.0, 19.0 ],
									"outlettype" : [ "int", "int", "int", "int" ],
									"id" : "obj-26",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 1440 3. 3.99",
									"fontname" : "Verdana",
									"numoutlets" : 1,
									"patching_rect" : [ 86.0, 245.0, 114.0, 19.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-25",
									"fontsize" : 10.0,
									"numinlets" : 6
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"fontname" : "Verdana",
									"numoutlets" : 1,
									"patching_rect" : [ 267.0, 109.0, 56.0, 19.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-24",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "screensize",
									"fontname" : "Verdana",
									"numoutlets" : 2,
									"patching_rect" : [ 267.0, 142.0, 63.0, 19.0 ],
									"outlettype" : [ "list", "list" ],
									"id" : "obj-23",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"fontname" : "Verdana",
									"numoutlets" : 2,
									"patching_rect" : [ 122.0, 201.0, 50.0, 19.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-22",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"fontname" : "Verdana",
									"numoutlets" : 2,
									"patching_rect" : [ 86.0, 181.0, 50.0, 19.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-20",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 2 @active 1",
									"fontname" : "Verdana",
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 100.0, 103.0, 19.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-13",
									"fontsize" : 10.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "mousestate",
									"fontname" : "Verdana",
									"numoutlets" : 5,
									"patching_rect" : [ 50.0, 128.0, 162.0, 19.0 ],
									"outlettype" : [ "int", "int", "int", "int", "int" ],
									"id" : "obj-11",
									"fontsize" : 10.0,
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 86.0, 326.0, 25.0, 25.0 ],
									"id" : "obj-36",
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 280.0, 326.0, 25.0, 25.0 ],
									"id" : "obj-37",
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 2 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 2 ],
									"destination" : [ "obj-25", 2 ],
									"hidden" : 0,
									"midpoints" : [ 321.166656, 233.0, 133.5, 233.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 3 ],
									"destination" : [ "obj-27", 2 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [ 131.5, 233.0, 289.5, 233.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Verdana",
					"numoutlets" : 2,
					"patching_rect" : [ 360.0, 160.0, 72.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-35",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Verdana",
					"numoutlets" : 2,
					"patching_rect" : [ 251.0, 159.0, 55.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-29",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arg 1: chaos_param: a parameter of the chaotic function with useful values from 0.0 to 4.0. Chaos occurs from 3.57 up. Don't use values outside this range if you don't want the external to blow up.\n\narg 2: freq: Frequency of calculation; if over the sampling rate, this is clamped to the sampling rate ",
					"linecount" : 4,
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 405.0, 186.0, 526.0, 55.0 ],
					"id" : "obj-6",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numoutlets" : 0,
					"patching_rect" : [ 356.0, 349.0, 384.0, 270.0 ],
					"id" : "obj-1",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.logistic~ 3.56 2000",
					"fontname" : "Verdana",
					"numoutlets" : 1,
					"patching_rect" : [ 251.0, 211.0, 128.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontsize" : 10.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"fontname" : "Verdana",
					"numoutlets" : 1,
					"patching_rect" : [ 251.0, 280.0, 50.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontsize" : 10.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 458.0, 284.0, 43.0 ],
					"id" : "obj-12",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Chaotic noise function.\nA noise generator based on the logistic map:\n\ny = chaosParam * y * (1.0 - y)",
					"linecount" : 4,
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 7.0, 42.0, 242.0, 55.0 ],
					"id" : "obj-9",
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.logistic~",
					"fontname" : "Verdana",
					"numoutlets" : 0,
					"patching_rect" : [ 11.0, 11.0, 136.0, 24.0 ],
					"id" : "obj-7",
					"fontface" : 3,
					"fontsize" : 14.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numoutlets" : 0,
					"patching_rect" : [ 251.0, 330.0, 45.0, 45.0 ],
					"id" : "obj-4",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"id" : "obj-10",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "biquad~ 1. -1. 0. -0.9997 0.",
					"fontname" : "Verdana",
					"numoutlets" : 1,
					"patching_rect" : [ 251.0, 247.0, 161.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-18",
					"fontsize" : 10.0,
					"numinlets" : 6
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 260.5, 272.0, 365.5, 272.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-3", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 1 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
