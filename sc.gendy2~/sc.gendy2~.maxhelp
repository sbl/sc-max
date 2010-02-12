{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 468.0, 89.0, 748.0, 727.0 ],
		"bglocked" : 0,
		"defrect" : [ 468.0, 89.0, 748.0, 727.0 ],
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
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"presentation_rect" : [ 688.0, 256.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 681.0, 257.0, 50.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-45",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 627.0, 257.0, 50.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-44",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Try some presets:",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 11.0, 176.0, 102.0, 19.0 ],
					"id" : "obj-40",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 103.0, 197.0, 68.0, 19.0 ],
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-24",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 571.0, 257.0, 50.0, 19.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-41",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1st arg: number of control points (in sc this defaults to 12) 1-n\n\nsc.gendy2~ understands attributes (check inspector)",
					"linecount" : 3,
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 103.0, 330.0, 367.0, 43.0 ],
					"id" : "obj-39",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sane freqs",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 322.0, 232.0, 64.0, 19.0 ],
					"id" : "obj-37",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0.0,
					"numoutlets" : 2,
					"patching_rect" : [ 498.0, 257.0, 50.0, 19.0 ],
					"maximum" : 1.0,
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-36",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0.0,
					"numoutlets" : 2,
					"patching_rect" : [ 423.0, 257.0, 50.0, 19.0 ],
					"maximum" : 1.0,
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-35",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 354.0, 257.0, 50.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-30",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 288.0, 257.0, 50.0, 19.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-25",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "preset",
					"numinlets" : 1,
					"numoutlets" : 4,
					"patching_rect" : [ 13.0, 198.0, 89.0, 16.0 ],
					"outlettype" : [ "preset", "int", "preset", "int" ],
					"id" : "obj-22",
					"preset_data" : [ 						{
							"number" : 1,
							"data" : [ 5, "obj-45", "flonum", "float", 0.31, 5, "obj-44", "flonum", "float", 1.17, 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.5, 5, "obj-35", "flonum", "float", 0.5, 5, "obj-30", "flonum", "float", 660.0, 5, "obj-25", "flonum", "float", 440.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-21", "flonum", "float", 1.0, 5, "obj-23", "number", "int", 1, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 2,
							"data" : [ 5, "obj-45", "flonum", "float", 0.436, 5, "obj-44", "flonum", "float", 0.0, 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.5, 5, "obj-35", "flonum", "float", 0.5, 5, "obj-30", "flonum", "float", 660.0, 5, "obj-25", "flonum", "float", 440.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-21", "flonum", "float", 1.0, 5, "obj-23", "number", "int", 1, 5, "obj-17", "number", "int", 1 ]
						}
, 						{
							"number" : 3,
							"data" : [ 5, "obj-45", "flonum", "float", 0.48, 5, "obj-44", "flonum", "float", 0.171, 5, "obj-41", "number", "int", 12, 5, "obj-36", "flonum", "float", 0.504, 5, "obj-35", "flonum", "float", 0.408, 5, "obj-30", "flonum", "float", 4792.0, 5, "obj-25", "flonum", "float", 461.0, 5, "obj-27", "flonum", "float", 0.309, 5, "obj-21", "flonum", "float", 0.543, 5, "obj-23", "number", "int", 1, 5, "obj-17", "number", "int", 2 ]
						}
, 						{
							"number" : 4,
							"data" : [ 5, "obj-45", "flonum", "float", 0.66, 5, "obj-44", "flonum", "float", 4.961, 5, "obj-41", "number", "int", 2, 5, "obj-36", "flonum", "float", 1.0, 5, "obj-35", "flonum", "float", 0.35, 5, "obj-30", "flonum", "float", 345.0, 5, "obj-25", "flonum", "float", 340.0, 5, "obj-27", "flonum", "float", 0.61, 5, "obj-21", "flonum", "float", 0.34, 5, "obj-23", "number", "int", 1, 5, "obj-17", "number", "int", 6 ]
						}
 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dcblock",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 176.0, 389.0, 52.0, 19.0 ],
					"id" : "obj-28",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "biquad~ 1. -1. 0. -0.9997 0.",
					"fontsize" : 10.0,
					"numinlets" : 6,
					"numoutlets" : 1,
					"patching_rect" : [ 13.0, 390.0, 161.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-29",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0.00001,
					"numoutlets" : 2,
					"patching_rect" : [ 216.0, 257.0, 50.0, 19.0 ],
					"maximum" : 1.0,
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-27",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0.00001,
					"numoutlets" : 2,
					"patching_rect" : [ 144.0, 257.0, 50.0, 19.0 ],
					"maximum" : 1.0,
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-21",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0,
					"numoutlets" : 2,
					"patching_rect" : [ 81.0, 257.0, 50.0, 19.0 ],
					"maximum" : 6,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-23",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "(values between 0-6)",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 16.0, 232.0, 119.0, 19.0 ],
					"id" : "obj-20",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"minimum" : 0,
					"numoutlets" : 2,
					"patching_rect" : [ 13.0, 257.0, 50.0, 19.0 ],
					"maximum" : 6,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-17",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pcontrol",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 118.0, 137.0, 51.0, 19.0 ],
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-6",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p details",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 172.0, 138.0, 53.0, 19.0 ],
					"hidden" : 1,
					"id" : "obj-5",
					"fontname" : "Verdana",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 188.0, 48.0, 681.0, 549.0 ],
						"bglocked" : 0,
						"defrect" : [ 188.0, 48.0, 681.0, 549.0 ],
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
									"maxclass" : "comment",
									"text" : "ampdist - Choice of probability distribution for the next perturbation of the amplitude of a control point. \n\nThe distributions are (adapted from the GENDYN program in Formalized Music):\n0- LINEAR\n1- CAUCHY\n2- LOGIST\n3- HYPERBCOS\n4- ARCSINE\n5- EXPON\n6- SINUS\nWhere the sinus (Xenakis' name) is in this implementation taken as sampling from a third party oscillator. See example below.\n\ndurdist- Choice of distribution for the perturbation of the current inter control point duration. \n\nadparam- A parameter for the shape of the amplitude probability distribution, requires values in the range 0.0001 to 1 (there are safety checks in the code so don't worry too much if you want to modulate!)\n\nddparam- A parameter for the shape of the duration probability distribution, requires values in the range 0.0001 to 1\n\nminfreq- Minimum allowed frequency of oscillation for the Gendy1 oscillator, so gives the largest period the duration is allowed to take on. \n\nmaxfreq- Maximum allowed frequency of oscillation for the Gendy1 oscillator, so gives the smallest period the duration is allowed to take on. \n\nampscale- Normally 0.0 to 1.0, multiplier for the distribution's delta value for amplitude. An ampscale of 1.0 allows the full range of  -1 to 1 for a change of amplitude.\n\ndurscale- Normally 0.0 to 1.0, multiplier for the distribution's delta value for duration. An ampscale of 1.0 allows the full range of  -1 to 1 for a change of duration.\n\ninitCPs- Initialise the number of control points in the memory. Xenakis specifies 12. There would be this number of control points per cycle of the oscillator, though the oscillator's period will constantly change due to the duration distribution. \n\nknum- Current number of utilised control points, allows modulation.  \n\na- parameter for Lehmer random number generator perturbed by Xenakis as in ((old*a)+c)%1.0\n\nc- parameter for Lehmer random number generator perturbed by Xenakis\n",
									"linecount" : 42,
									"fontsize" : 10.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 5.0, 2.0, 638.0, 517.0 ],
									"id" : "obj-13",
									"fontname" : "Verdana Bold"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 6.0, 246.0, 25.0, 25.0 ],
									"outlettype" : [ "" ],
									"hidden" : 1,
									"id" : "obj-3",
									"comment" : ""
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 259.0, 459.0, 315.0, 128.0 ],
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.gendy2~ 12",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 13.0, 337.0, 86.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.4",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 13.0, 457.0, 45.0, 19.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 3.0, 654.0, 279.0, 43.0 ],
					"id" : "obj-12",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.gendy2~",
					"fontface" : 3,
					"fontsize" : 16.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 11.0, 125.0, 25.0 ],
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 13.0, 498.0, 45.0, 45.0 ],
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"rounded" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"id" : "obj-10"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 70.0, 139.0, 50.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "From SC Helpfiles. Original SC implementation by Nick Collins (sicklincoln.org)\n\nSee Gendy1 help file for background. This variant of GENDYN is closer to that presented \nin Hoffmann, Peter. (2000) The New GENDYN Program. Computer Music Journal 24:2, pp 31-38. \n\nTechnical notes: random walk is of the amplitude and time delta, not the amp and time directly. The amplitude step random walk uses a lehmer style number generator whose parameters are accessible. \n\nDetails ->",
					"linecount" : 9,
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 42.0, 605.0, 116.0 ],
					"id" : "obj-9",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampdist $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 13.0, 289.0, 65.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "durdist $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 81.0, 289.0, 60.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-15",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "adparam $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 144.0, 289.0, 69.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-16",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ddparam $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 216.0, 289.0, 69.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "minfreq $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 288.0, 289.0, 63.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "maxfreq $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 354.0, 289.0, 66.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-19",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampscale $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 423.0, 289.0, 72.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "durscale $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 498.0, 289.0, 67.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "knum $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 571.0, 289.0, 53.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "a $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 627.0, 289.0, 50.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Verdana"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "c $1",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 681.0, 289.0, 50.0, 17.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-31",
					"fontname" : "Verdana"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 690.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 636.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 90.0, 435.0, 123.0, 424.0, 133.0, 468.0, 203.0, 496.0, 158.0, 566.0, 234.0, 596.0, 426.0, 646.0, 597.0, 612.0, 638.0, 511.0, 712.0, 428.0, 597.0, 371.0, 619.0, 425.0, 509.0, 445.0, 537.0, 409.0, 456.0, 393.0, 426.0, 432.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 580.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 363.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 297.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 90.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 22.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 153.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 225.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 432.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 507.5, 321.0, 22.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-2", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
