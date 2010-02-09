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
					"text" : "freq",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 363.0, 142.0, 78.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-15",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "chaos",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 259.0, 142.0, 78.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-14",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p mouse_coords",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 251.0, 118.0, 123.0, 19.0 ],
					"numinlets" : 0,
					"id" : "obj-38",
					"fontname" : "Verdana",
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
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 280.0, 247.0, 153.0, 19.0 ],
									"numinlets" : 6,
									"id" : "obj-27",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0 0 0 0",
									"numoutlets" : 4,
									"outlettype" : [ "int", "int", "int", "int" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 267.0, 185.0, 86.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-26",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 1440 3. 3.99",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 86.0, 245.0, 114.0, 19.0 ],
									"numinlets" : 6,
									"id" : "obj-25",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 267.0, 109.0, 56.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-24",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "screensize",
									"numoutlets" : 2,
									"outlettype" : [ "list", "list" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 267.0, 142.0, 63.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-23",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 122.0, 201.0, 50.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-22",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 86.0, 181.0, 50.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-20",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 2 @active 1",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 50.0, 100.0, 103.0, 19.0 ],
									"numinlets" : 2,
									"id" : "obj-13",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "mousestate",
									"numoutlets" : 5,
									"outlettype" : [ "int", "int", "int", "int", "int" ],
									"fontsize" : 10.0,
									"patching_rect" : [ 50.0, 128.0, 162.0, 19.0 ],
									"numinlets" : 1,
									"id" : "obj-11",
									"fontname" : "Verdana"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 86.0, 326.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"id" : "obj-36",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 280.0, 326.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"id" : "obj-37",
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
						"default_fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 355.0, 159.0, 50.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-35",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 251.0, 159.0, 50.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-29",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arg 1: chaos_param: a parameter of the chaotic function with useful values from 0.0 to 4.0. Chaos occurs from 3.57 up. Don't use values outside this range if you don't want the external to blow up.\n\narg 2: freq: Frequency of calculation; if over the sampling rate, this is clamped to the sampling rate ",
					"linecount" : 6,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 421.0, 195.0, 428.0, 79.0 ],
					"numinlets" : 1,
					"id" : "obj-6",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numoutlets" : 0,
					"patching_rect" : [ 356.0, 349.0, 384.0, 270.0 ],
					"numinlets" : 2,
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.logistic~ 3.56 2000",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 251.0, 211.0, 123.0, 19.0 ],
					"numinlets" : 2,
					"id" : "obj-3",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 250.0, 249.0, 45.0, 19.0 ],
					"numinlets" : 2,
					"id" : "obj-2",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 5.0, 458.0, 279.0, 43.0 ],
					"numinlets" : 1,
					"id" : "obj-12",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Chaotic noise function.\nA noise generator based on the logistic map:\n\ny = chaosParam * y * (1.0 - y)",
					"linecount" : 4,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 7.0, 42.0, 237.0, 55.0 ],
					"numinlets" : 1,
					"id" : "obj-9",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.logistic~",
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 16.0,
					"patching_rect" : [ 11.0, 11.0, 125.0, 25.0 ],
					"numinlets" : 1,
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numoutlets" : 0,
					"patching_rect" : [ 250.0, 320.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 0,
					"numoutlets" : 0,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"numinlets" : 1,
					"id" : "obj-10"
				}

			}
 ],
		"lines" : [ 			{
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
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 260.5, 237.0, 365.5, 237.0 ]
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
